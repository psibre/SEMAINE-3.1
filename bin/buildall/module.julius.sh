
#################### Package Configuration    #########################
JU_BASE="julius-4.1.2"
#SP_URL="http://www.portaudio.com/archives/pa_stable_v19_20071207.tar.gz"
#SP_URL="http://www.portaudio.com/archives/pa_snapshot.tgz"
JU_URL="http://osdn.dl.sourceforge.jp/julius/37582/julius-4.1.2.tar.gz"

register_build "julius" "$JU_URL" "$JU_BASE" "func_build_julius" $1

#######################################################################

# build_nr var must be set before this function is called
function func_build_julius {



#    cp $DOWNLOAD_PREFIX/julius.semaine.patch .
#    patch -p0 < julius.semaine.patch
    JROOT="$SEMAINE_ROOT/c++/src/tum";
    cp $JROOT/julius4.mod/vocabulary.h libsent/include/sent
    cp $JROOT/julius4.mod/adin.h libsent/include/sent
    cp $JROOT/julius4.mod/recog.h libjulius/include/julius
    cp $JROOT/julius4.mod/jconf.h libjulius/include/julius
    cp $JROOT/julius4.mod/plugin.h libjulius/include/julius
    cp $JROOT/julius4.mod/realtime-1stpass.c libjulius/src
    cp $JROOT/julius4.mod/recogmain.c libjulius/src
    cp $JROOT/julius4.mod/plugin.c libjulius/src
    cp $JROOT/julius4.mod/m_info.c libjulius/src
    cp $JROOT/julius4.mod/vsegment.c libsent/src/phmm
    cp $JROOT/julius4.mod/word_align.c libjulius/src

    export CFLAGS="-DEXTERNAL_FV $DBGFLAGS -fPIC $OFLAGS"

    
    doconf=$1
    if test -e "Makefile" ; then
      echo "found Makefile, not configuring (use 'clean' parameter to override)"
    else 
      doconf="clean";
    fi

    if test "x$doconf" = "xclean" ; then

    cat Makefile.in | sed 's/^CONFIG_SUBDIRS.\+$/CONFIG_SUBDIRS=julius libjulius libsent/' > Makefile.in2
    cat Makefile.in2 | sed 's/^SUBDIRS.\+$/SUBDIRS=julius libjulius libsent/' > Makefile.in3
    rm Makefile.in2
    mv Makefile.in Makefile.in.bak
    mv Makefile.in3 Makefile.in
    cat configure | sed 's/^subdirs=.\+$/subdirs="libsent libjulius julius"/' > configure2
    cat configure2 | sed 's/^  for ac_config_dir in .\+$/  for ac_config_dir in julius libjulius libsent; do/' > configure3
    rm configure2
    mv configure configure.bak
    mv configure3 configure
    chmod +x configure


    ./configure --prefix=$INSTALL_PREFIX  

    cat libsent/include/sent/config.h | sed 's/.define HAVE_ESD_H 1$//' > libsent/include/sent/config.h2
    cat libsent/include/sent/config.h2 | sed 's/.define HAVE_ZLIB 1$//' > libsent/include/sent/config.h3
    cat libsent/include/sent/config.h3 | sed 's/.define HAVE_LIBSNDFILE 1$//' > libsent/include/sent/config.h4
    cat libsent/include/sent/config.h4 | sed 's/.define USE_MIC 1$//' > libsent/include/sent/config.h5
    echo "#define ZCAT \"gzip -d -c\"" >> libsent/include/sent/config.h5

    mv libsent/include/sent/config.h libsent/include/sent/config.h.bak
    rm libsent/include/sent/config.h2
    rm libsent/include/sent/config.h3
    rm libsent/include/sent/config.h4
    mv libsent/include/sent/config.h5 libsent/include/sent/config.h

    cat libsent/Makefile | sed 's/ src\/adin\/adin_.*/ /g' > libsent/Makefile.1
    mv libsent/Makefile libsent/Makefile.bak
    mv libsent/Makefile.1 libsent/Makefile

    if test "x$?" != "x0" ; then
      return 1;
    fi

    fi

    if test "x$1" = "xclean" ; then
      make clean 
      if test "x$?" != "x0" ; then
        return 1;
      fi
    fi
    
    make && make install
    if test "x$?" != "x0" ; then
      return 1;
    fi
    
    addConf "JULIUSPATH" "${builds_dirs[$build_nr]}"

    return 0;
}


