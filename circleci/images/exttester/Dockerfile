ARG PG_MAJOR

FROM postgres:$PG_MAJOR

COPY files /tmp

RUN set -ex; \
	ln -s /tmp/sbin/* /usr/local/sbin; \
	ln -s /tmp/bin/* /usr/local/bin; \
  ln -s /tmp/etc/* /etc/gdb/gdbinit; \
	install-testdeps

WORKDIR /home/circleci
