## System Power Manager
## Table of contents
- [System Power Manager](#system-power-manager)
- [Table of contents](#table-of-contents)
  - [What is System Power Manager?](#what-is-system-power-manager)
  - [What does it do?](#what-does-it-do)
  - [Why it exists?](#why-it-exists)
  - [Dependencies](#dependencies)
    - [How can I install the dependencies](#how-can-i-install-the-dependencies)
  - [Build and install](#build-and-install)
    - [Build](#build)
      - [Getting the sources](#getting-the-sources)
      - [Compilation](#compilation)
    - [Installation](#installation)
  - [Run](#run)
  - [How can I use it on dwm](#how-can-i-use-it-on-dwm)
    - [Defining a constant](#defining-a-constant)
    - [Adding a keyboard shorcut for shutdown](#adding-a-keyboard-shorcut-for-shutdown)
    - [If you have a laptop computer](#if-you-have-a-laptop-computer)
      - [Define a constant](#define-a-constant)
      - [Adding a keyboard shortcut for sleeping feature](#adding-a-keyboard-shortcut-for-sleeping-feature)
  - [Command line options](#command-line-options)
  - [Keyboard shortcuts](#keyboard-shortcuts)
  - [Note](#note)
  - [Some screenshots](#some-screenshots)

### What is System Power Manager?
System Power Manager (from now SPM) as its name suggests this program aims to manage some energy options.

### What does it do?
SPM can shutdown, reboot, hibernate (if your system allows it), and suspend your computer.
Also it can monitors your battery's charge.
This latest, with the objective of putting your computer to sleep if it has 15% or less.
So, avoid that you can lose your current work.

### Why it exists?
I have developed it to use on [dwm](https://github.com/brookiestein/dwm)
since it was very heavy open a terminal, enter the root password, etc., for do
things that shouldn't give much work to perform, like e.g. shutdown or reboot the system.

**Even though I developed it for satisfy a personal need in dwm, you can use it however and wherever you want.**

### Dependencies:
```
autoreconf
make (for compiling)
dbus
gtk3 (If you want to compile the GUI)
elogind (It's included in systemd)
libnotify
```
#### How can I install the dependencies?
Depending on your Linux distribution use the corresponding command:
| Distribution                     | Command                                                                                      |
|:---------------------------------|----------------------------------------------------------------------------------------------|
|Debian/Ubuntu and derivatives     |`apt install make dbus libgtk-3-dev elogind libnotify-dev autotools-dev`                                    |
|Fedora/CentOS/RHEL                |`dnf install make dbus-glib gtk3-devel elogind libnotify autoconf`                                     |
|Gentoo/Funtoo and derivatives     |`emerge --ask sys-devel/make sys-apps/dbus x11-libs/gtk+ x11-libs/libnotify sys-auth/elogind sys-devel/autoconf` |
|Arch Linux/Manjaro and derivatives|`pacman -S make dbus gtk3 elogind libnotify autoconf`                                                  |

**Remember to execute these commands as user root**

### Build and install
#### Build
##### Getting the sources
First get the sources:
```
$ git clone https://github.com/brookiestein/spm
```
##### Compilation
After that proceed to the compilation:
```
$ autoreconf --install
$ ./configure
$ make
```
Or
```
$ autoreconf --install
$ ./configure --enable-gui
$ make
```
If you want SPM to be built with GUI support.

You may want to specify whether SPM asks you for a screen locker or uses a default one with: `--enable-locker` and `--enable-default-locker=locker`. For example, if you want to use `slock` as your default screen locker in SPM, you should do this:
```
$ ./configure --enable-default-locker=slock
```
`--enable-default-locker` works right out of the box with the GUI, so:
```
$ ./configure --enable-default-locker=slock --enable-gui
```
Would be a great combination.

### Installation
Then install:
```
# make install
```
If you compiled SPM with GUI support, you must do the following:
```
# make res
```

### Run
For launching the program: execute in a console the next command (If was compiled with GUI support):
```
$ spm
```
To learn which options are available:
```
$ spm --help
```

### How can I use it on dwm
If you want to add it as a keyboard shortcut in dwm, add this to your config.h:

#### Defining a constant
First, define a constant with the command:
```
static const char *spm[]              = { "spm", NULL };
```
Notice that using SPM without any of its parameters means that you want to use its GUI.

#### Adding a keyboard shorcut for shutdown
Then, add the keyboard shortcut and recompile your dwm:
```
static Key keys[] = {
        ...
        { 0,                            XF86XK_PowerOff,spawn,     {.v = spm } },
        ...
};
```
Basically if you press the poweroff button, located physically in your computer, SPM's GUI
will appear showing you the power options you can do with it.

### If you have a laptop computer
If you have a laptop, you may wish to add the suspend's feature it may have.
For example I have a Lenovo Thinkpad laptop and it has that feature by pressing: FN + F4.
That key combination, in an X system, is called XF86XK_Sleep, so add the following lines
to your config.h file.

#### Define a constant
As is obvious, first declare a variable (The name is indifferent):
```
static const char *suspend[]              = { "spm", "--suspend", NULL };
```

#### Adding a keyboard shortcut for sleeping feature
Then, add the keyboard shortcut:
```
static Key keys[] = {
        ...
        { 0,                            XF86XK_Sleep,spawn,     {.v = suspend } },
        ...
};
```
**With this, dwm "will simulate" the popular keyboard shortcut for suspending your computer ;-).**

### Command line options
The following command line options were added if you don't want GUI.
|Option              | Description                                             |
|:-------------------|---------------------------------------------------------|
|Power options:      |                                                         |
|-p, --poweroff      | Shutdowns the computer.                                 |
|-H, --hibernate     | Hibernates the computer.                                |
|-r, --reboot        | Restarts the computer.                                  |
|-s, --suspend       | Suspends the computer.                                  |
|                                                                              |
|Guardian options:   |                                                         |
|-d, --daemon        | Run as daemon. Useful when combined with -m, and -f.    |
|-m, --monitor       | Checks whether the battery's charge is greater than 15%.|
|                    | If not, then suspends the computer.                     |
|                    |                                                         |
|File options:       |                                                         |
|-f, --file=file     | File where log is going to be saved.                    |
|                    |                                                         |
|Help options:       |                                                         |
|-h, --help          | Shows this help and exit.                               |
|                                                                              |
|Verbose options:                                                              |
|-D, --debug         | Shows debug information.                                |
|-v, --verbose       | Be verbose.                                             |
|                                                                              |
|Misc options:                                                                 |
|-l, --locker=locker | Screen locker which would be run when suspending.       |
|-w, --wait=sec      | Time to wait before performing a power option.          |
|-V, --version       | Shows program version and exits.                        |
|                                                                              |
|GUI Options:                                                                  |
|Run without arguments to show the GUI.                                        |

You can use these parameters for a simple and quick solution. On the other hand, 
if you want to use the GTK interface, then do not offer any parameters.

### Keyboard shortcuts
If you want to use the GUI, the following keyboard shortcuts may interest you:
|Keyboard shortcut              | Description             |
|:------------------------------|-------------------------|
|Escape                         | Closes the application. |
|p                              | Turns off the system.   |
|h                              | Hibernates the system.  |
|r                              | Reboots the system.     |
|s                              | Suspends the system.    |

### Note
Also you can declare variables for use the shortcuts by way of that you
don't need execute the GUI to perform any of the options SPM allows you to do.

How to perform it? For sure you know ;-)

### Some screenshots

![](screenshots/gui.png)
![](screenshots/cli.png)

