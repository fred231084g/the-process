# buildpack-deps:stretch
FROM buildpack-deps@sha256:8efabde824fe7c9cb1eaacaecc3b397706fe49b7fc08a1842b3799dcdf08f7c2

COPY files /tmp

RUN set -ex; \
	ln -s /tmp/sbin/* /usr/local/sbin/; \
	ln -s /tmp/bin/* /usr/local/bin/; \
	ln -s /tmp/etc/* /etc/dpkg/dpkg.cfg.d/; \
	install-extdeps

USER circleci
WORKDIR /home/circleci

RUN mkdir -p /tmp/_circleci_local_workspace
