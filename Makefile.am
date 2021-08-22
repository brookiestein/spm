spm_SOURCES = battery_monitor.c format.c get_time.c logger.c \
	      power_options.c run_locker.c spm.c
bin_PROGRAMS = spm

if GUI
spm_SOURCES += gui.c
endif

if LOCKER
if GUI
spm_SOURCES += ask_for_locker.c
endif # GUI
endif # LOCKER

CP = cp -rf
MKDR = mkdir -p
TGT = spm
PFX = /usr/share/spm
RES = resources

res :
	$MKDR ${PFX}
	$CP $RES $PFX
