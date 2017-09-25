Watara Supervision Emulator for Android
==============

This is a port of potator emu for android. Potator is a watara supervision emulator in SDL.

### sources:
* https://github.com/OpenEmu/Potator-Core
* http://en.wildservices.net/2013/10/making-libsdl-2-apps-on-android.html
* http://libsdl.org
* http://supermariowar-android.googlecode.com/

### compile and build:

To build this project there are 2 prerequisites

- NDK https://developer.android.com/ndk/downloads/index.html
- SDL sources http://libsdl.org/download-2.0.php#source

1. place the `SDL` folder in a sibling directory of the `android-project`, e.g. inside `SuperVisionEmu`
2. check `jni/src/Application.mk` and make sure the `NDK_MODULE_PATH` variable exists and points to the `SDL` directory. We are doing the called static linking to SDL library, see the `Build an app with static linking of libSDL` paragraph of `readme-android` inside the `SDL` directory.
3. run `ndk-build` from a command line inside the `android-project` directory
4. after build is finished, a file `libmain.so` should be present inside the `libs\armeabi-xxx` directory.
5. execute the gradle build task, take the `apk` produced from the `output` directory and install it on your android device.
