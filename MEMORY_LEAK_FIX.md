# Snapshot类内存泄漏修复

## 问题描述

在原始的 `Snapshot` 类实现中存在严重的内存泄漏问题：

1. **takeSnapshot()方法中的内存分配**：
   - 为不同CPU架构分配内存：`cpu_x8664`, `cpu_x86`, `cpu_AArch64`, `cpu_Arm32`
   - 分配AST上下文：`astCtx`

2. **缺失的内存释放**：
   - `resetEngine()` 方法中没有释放CPU对象
   - `~Snapshot()` 析构函数为空，没有清理
   - 多次调用 `takeSnapshot()` 会导致旧对象泄漏

## 修复内容

### 1. 构造函数初始化
```cpp
Snapshot::Snapshot() {
    this->locked = true;
    this->snapshotTaintEngine = nullptr;
    this->snapshotSymEngine = nullptr;
    this->astCtx = nullptr;                    // 新增
    this->cpu_x8664 = nullptr;                 // 新增
    this->cpu_x86 = nullptr;                   // 新增
    this->cpu_AArch64 = nullptr;               // 新增
    this->cpu_Arm32 = nullptr;                 // 新增
    this->mustBeRestore = false;
    this->snapshotTaken = false;
}
```

### 2. 析构函数清理
```cpp
Snapshot::~Snapshot() {
    // 确保在析构时清理所有分配的内存
    resetEngine();
}
```

### 3. resetEngine()方法完善
```cpp
void Snapshot::resetEngine(void) {
    if (!this->snapshotTaken)
        return;

    this->memory.clear();

    // 原有的清理
    delete this->snapshotSymEngine;
    this->snapshotSymEngine = nullptr;
    delete this->snapshotTaintEngine;
    this->snapshotTaintEngine = nullptr;

    // 新增：释放AST上下文
    delete this->astCtx;
    this->astCtx = nullptr;

    // 新增：释放CPU状态对象
    switch (tritonCtx.getArchitecture()) {
    case triton::arch::ARCH_X86_64:
        delete this->cpu_x8664;
        this->cpu_x8664 = nullptr;
        break;
    case triton::arch::ARCH_X86:
        delete this->cpu_x86;
        this->cpu_x86 = nullptr;
        break;
    case triton::arch::ARCH_AARCH64:
        delete this->cpu_AArch64;
        this->cpu_AArch64 = nullptr;
        break;
    case triton::arch::ARCH_ARM32:
        delete this->cpu_Arm32;
        this->cpu_Arm32 = nullptr;
        break;
    default:
        break;
    }

    this->snapshotTaken = false;
    // ... 其余代码
}
```

### 4. takeSnapshot()方法防护
```cpp
void Snapshot::takeSnapshot() {
    // 如果已经有快照，先清理旧的内存以防止泄漏
    if (this->snapshotTaken) {
        resetEngine();
    }
    
    // ... 其余分配代码
}
```

## 修复效果

1. **防止内存泄漏**：所有动态分配的内存都会被正确释放
2. **防止重复分配**：多次调用 `takeSnapshot()` 不会泄漏旧对象
3. **安全析构**：对象销毁时自动清理所有资源
4. **架构无关**：支持所有CPU架构的正确清理

## 测试建议

1. 多次创建和删除快照
2. 在不同CPU架构下测试
3. 使用内存检测工具（如Valgrind、AddressSanitizer）验证
4. 长时间运行测试确保无累积泄漏

## 注意事项

- 修复保持了原有的功能逻辑不变
- 所有指针在释放后都设置为nullptr，防止悬空指针
- 在 `takeSnapshot()` 开始时检查并清理旧快照，确保不会有重复分配
