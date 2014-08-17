# -*- Makefile -*-, you silly Emacs!
#From the dpatch debian package.
#Copyright (C) 2002, 2003  Gergely Nagy, Joerg Jaspert
#Released under the GPL

DPATCH_ARCH	 = $(shell dpkg-architecture -qDEB_BUILD_ARCH)
PATCHLIST_ALL	 = $(shell cat debian/patches/00list | grep -v ^\# )
PATCHLIST_ARCH	 = $(shell cat debian/patches/00list.${DPATCH_ARCH} | grep -v ^\# 2>/dev/null)
PATCHLIST	?= $(sort ${PATCHLIST_ALL} ${PATCHLIST_ARCH})

UNPATCHLIST	 = $(shell echo ${PATCHLIST} | tr ' ' '\n' | tac)

patch: patch-stamp
patch-stamp:
	test -d debian/patched || install -d debian/patched
	@echo "Patches applied in the Debian version of ${PACKAGE}:" > $@T
	@for patch in ${PATCHLIST}; do \
		patchdir=$${patch%/*}; \
		test "x$$patchdir" = "x$$patch" || \
			install -d debian/patched/$$patchdir; \
                stamp=debian/patched/$${patch%%.dpatch}.dpatch; \
		test -e debian/patches/$$patch || ext=.dpatch; \
                patch=debian/patches/$$patch$$ext; \
                author=`sed -n "s,^#* *.*dpatch by *,,p" $$patch`; \
                test -x $$patch || chmod +x $$patch; \
		if test -f $$stamp; then \
                        echo "$$patch already applied."; \
                        echo -e "\n$$patch ($$author):" >> $@T; \
                        sed -n 's/^#* *DP: */  /p' $$patch >> $@T; \
                else \
                        echo -n "applying patch $$patch..."; \
                        if $$patch -patch >$$stamp.new 2>&1; then \
                                mv $$stamp.new $$stamp; \
                                touch $$stamp; \
                                echo -e "\n$$patch ($$author):" >> $@T; \
                                sed -n 's/^#* *DP: */  /p' $$patch >> $@T; \
                                echo " ok."; \
                        else \
                                echo " failed."; \
                                exit 1; \
                        fi; \
                fi; \
	done
	mv -f $@T $@

unpatch:
	@for patch in ${UNPATCHLIST}; do \
                stamp=debian/patched/$${patch%%.dpatch}.dpatch; \
		test -e debian/patches/$$patch || ext=.dpatch; \
                patch=debian/patches/$$patch$$ext; \
                test -x $$patch || chmod +x $$patch; \
                if test -f $$stamp; then \
                        echo -n "reverting patch $$patch..."; \
                        if $$patch -unpatch 2>&1 >/dev/null; then \
                                rm -f $$stamp; \
                                echo " ok."; \
                        else \
                                echo " failed."; \
                                exit 1; \
                        fi; \
                fi; \
	done
	rm -rf patch-stamp patch-stampT debian/patched
