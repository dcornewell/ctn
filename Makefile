install:
	cd apps/apps_test;	make install
	cd runtime;		make install
	cd facilities;		make links
	cd libsrc;		make install
	cd apps;		make install

install-clean:
	cd apps/apps_test;	make install
	cd facilities;		make links
	cd libsrc;		make install-clean
	cd apps;		make install-clean

install.mb:
	cd apps/apps_test;	make install.mb
	cd facilities;		make links.mb
	cd libsrc;		make install.mb
	cd external;		make install
	cd apps;		make install.mb
	cd cgi_apps;		make install
	cd www;			make install
	cd runtime;		make install

rebuild:
	make clean
	make install

tools:
	cd apps/apps_test;	make install
	cd runtime;		make install
	cd facilities;		make links
	cd libsrc;		make install
	cd apps;		make tools

clean:
	cd libsrc;		make clean
	cd apps;		make clean
	cd cgi_apps;		make clean

clean.mb:
	cd libsrc;		make clean
	cd external;		make clean
	cd apps;		make clean
	cd cgi_apps;		make clean
