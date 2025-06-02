# Port Ponce to newer IDA version (for maintaners)

Ponce supports any IDA version >= 7.0. If a new version of IDA pops up this are the steps to update Ponce to work with it:

1) Use your `ida.key` to request the newest IDA version on [this](https://www.hex-rays.com/updida/) site
2) In the email HexRays sends there will be a user and password and a link to download the SDK. Download it.
3) We need to add the content of `C:\Program Files\IDA Pro 7.X\plugins\hexrays_sdk\include` into the `include` path of the downlaoded SDK zip from the prevoius step. To obtain the files to add (should be one called `hexrays.hpp` and maybe others) you have to install IDA (remember it needs to be bundled with the decompiler for this file to be present)
4) Zip up all the IDA versions SDKs in a file called `IDA_SDKs.zip` and upload it to a site that supports direct download (i.e. bublup.com).
5) Check that the IDA APIs have not change and Ponce compiles.
