.PHONY : all clean delete rebuild
all :
	cd /home/darknightghost/project/AntiPkgLoss/src/client;make all
	cd /home/darknightghost/project/AntiPkgLoss/src/server;make all
clean :
	cd /home/darknightghost/project/AntiPkgLoss/src/client;make clean
	cd /home/darknightghost/project/AntiPkgLoss/src/server;make clean
delete :
	cd /home/darknightghost/project/AntiPkgLoss/src/client;make delete
	cd /home/darknightghost/project/AntiPkgLoss/src/server;make delete
rebuild :
	cd /home/darknightghost/project/AntiPkgLoss/src/client;make rebuild
	cd /home/darknightghost/project/AntiPkgLoss/src/server;make rebuild
