# Danger Force #
## A bullet hell homebrew game made using legacy console SDKs ##

### ... Why!? Because... because it's kinda cool I dunno. ###

Danger Force is a game I've been writing for a while (and continue to write, privately) for desktop platforms using C. I like C because I like writing games the old fashion way -- I don't really care for new and "_better_" tricks of modern langs. Anyway, I decided it would be a fun challenge to make this game for classic systems because it's ironically not all that hard and I think the limitations are fun to work around. I plan to write this 'legacy' version of Danger Force and port it to PSX, N64, and the Sega Dreamcast. Maybe afterwards the real game will get done lmao.

I'm open sourcing it for other peoples' reference only. I don't really want this to be a collaborative effort or anything, but feel free to take snippets as you like from my platform code. I noticed there's relatively little help for people who want to program this way, so with what little I offer I hope it helps someone down the line. My code is pretty messy but I try to document a lot. Hope you like it!

### What if I want to play the thing? ###

You want to _PLAY_ the thing!? Good gravy! OK, well, it'd be a hard sell to ask you to compile it yourself, which involves not only obtaining the compiler but using it on 32-bit windows. No, seriously, I'm compiling in a Windows XP 32-bit virtual machine. Instead what I'll be doing is whenever I get done with a certain part I'll just zip it up and put it in releases. 

* PSX files will be a PS-EXE file that can be loaded into an emulator. I test using ePSXe. You can theoretically put it on modded PS1 hardware, but I won't be doing that until the end. That's all you, B!

* N64 files will be N64 roms that can be played on an emulator. I'm not really at this part yet, so, yeah, don't count on that for a while.

* Dreamcast is the same situation. Probably ISOs. 

### Actually important credits section ###

Being that this is now almost a 20 year old API I needed a lot of help getting this sucker going. Here I'm going to (((try))) and compile all of the help that I used for this project.

* [PSXDEV.NET](http://psxdev.net) Basically the best resource for PS1 development in these modern times
* [PSXFINDER](https://github.com/apiraino/psx_pxfinder) Helped me out with finding pixel coordinates
* [Orion](http://onorisoft.free.fr/retro.htm?psx/tutorial/tuto.htm) Amazing guy with amazing articles about old console dev. Really great stuff and I admittedly lifted a good bit of platform code out of his PsxLib library. This article is probably WAY better than anything you'll find here, and that's my justifcation for using his library. DESPITE how it looks, by the way, it's a relatively new site.

### I have a cool idea and you should listen ###

Sweet dude hit me up

### Why do you waste time writing readmes I'm going to pull request and do comment commits anyway guy ###

[GOOD QUESTIONnnnnnnnnnnnnnnn..................](http://idunnolol.com/)
