libsdl-android
==============

This is a custom Android project template for libSDL 2 with libSDL 2 patches to fix or workaround some SDL bugs.

Usage instructions:

Requires libSDL 2.0.1 rev. 7804. Get it from the [Mercurial repository][1] or get the [latest source snapshot][1].

Apply the patches from libSDL.patch on top of your libSDL source tree:

	patch -p1 <libSDL.patch

The android-project folder contains a fully working project template. Copy this folder to a new location and start hacking.

See the instructions in libSDL's README-android.txt on how to build the project.

Implementation details: http://en.wildservices.net/2013/10/making-libsdl-2-apps-on-android.html

TODO:

- The template code needs to be cleaned up: use `return` instead of `exit()`, make proper initialisation/finalisation calls, reduce code size a bit.
- The main native thread does not receive `SDL_QUIT` when the OS wants to kill the app ([bug 2041](https://bugzilla.libsdl.org/show_bug.cgi?id=2041)).
- If we exit from the main native thread, the app does not terminate.
- Port the iOS SDL tests to Android.

[1]: http://libsdl.org/hg.php