
#--------------------------------------------------------------------

ifeq ($(origin version), undefined)
	version = 0.8
endif

#--------------------------------------------------------------------

all:
	@( cd spserver; make )

ssl:
	@( cd openssl;  make )

dist: clean spserver-$(version).src.tar.gz

spserver-$(version).src.tar.gz:
	@find . -type f | grep -v CVS | grep -v .svn | sed s:^./:spserver-$(version)/: > MANIFEST
	@(cd ..; ln -s spserver spserver-$(version))
	(cd ..; tar cvf - `cat spserver/MANIFEST` | gzip > spserver/spserver-$(version).src.tar.gz)
	@(cd ..; rm spserver-$(version))

clean:
	@( cd spserver; make clean )
	@( cd openssl;  make clean )
	@( cd matrixssl;  make clean )
	@( cd sptunnel;  make clean )

