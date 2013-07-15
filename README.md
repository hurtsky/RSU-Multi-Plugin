# RSU-Multi-Plugin #

A Plugin for RagnarokOnline Patcher Lite (RSU). Allows for patching multiple GRF files (kRO, kRO:RE & Custom) using just one patch client instead of multiple.


# Set Up / Compiling #
- Open multi.cpp file
- Change settings for `PATCH_CUSTOM` under the `Set_Patch_Info()` function.
- Change settings for client EXE / Command under `RSU_PLUG_EVENT_INITDONE` in the `Plugin_OnEvent()` function.
- Open your Visual Studio Command line.
- Navigate to the where the files are located.
- Run multi.bat
- It should compile a file called **multi.rpi**, throw this file into your client directory with [RSU Pure](http://nn.nachtwolke.com/dev/rel/rsu/2012-04-15rsu-full.zip).
