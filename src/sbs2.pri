QT += network

macx {
    message("OSX")

    LIBS += -framework IOKit -framework CoreFoundation
    SOURCES += $$PWD/platform/osx/hid.c
    HEADERS +=  $$PWD/platform/osx/hidapi.h
}

unix:!macx:!android-g++ {
    message("LINUX")

    LIBS += `pkg-config libudev --libs` -lrt
    SOURCES += $$PWD/platform/linux/hid.c
    HEADERS +=  $$PWD/platform/linux/hidapi.h
}

win32 {
    message("WINDOWS")

    LIBS += -lSetupAPI
    SOURCES += $$PWD/platform/windows/hid.c
    HEADERS +=  $$PWD/platform/windows/hidapi.h
}

macx:exists($$PWD/../../binary_decryptor/libsbs2emotivdecryptor-macx.a) {
    message("Binary decryptor for Mac found!")
    LIBS += -L$$PWD/../../binary_decryptor/ -lsbs2emotivdecryptor-macx
} else:android:exists($$PWD/../../binary_decryptor/libsbs2emotivdecryptor-android.a) {
    message("Binary decryptor for Android found!")
    LIBS += -L$$PWD/../../binary_decryptor/ -lsbs2emotivdecryptor-android
} else:unix:!android:!macx:exists($$PWD/../../binary_decryptor/libsbs2emotivdecryptor-unix.a) {
    message("Binary decryptor for Linux found!")
    LIBS += -L$$PWD/../../binary_decryptor/ -lsbs2emotivdecryptor-unix
} else:win32:exists($$PWD/../../binary_decryptor/libsbs2emotivdecryptor-windows.a) {
    message("Binary decryptor for Windows found!")
    LIBS += -L$$PWD/../../binary_decryptor/ -lsbs2emotivdecryptor-windows
} else:exists($$PWD/../../sbs2emotivdecryptor/) {
    message("Found decryptor sources. Be careful what you distribute.")
    SOURCES += $$PWD/../../sbs2emotivdecryptor/sbs2emotivdecryptor.cpp
    INCLUDEPATH += $$PWD/hardware/emotiv
} else {
    message("No suitable binary decryptor was found, using dummy decryptor instead. Expect random noise")
    SOURCES += $$PWD/hardware/emotiv/sbs2emotivdecryptor_dummy.cpp
}

INCLUDEPATH += $$PWD

SOURCES += $$PWD/FFTReal.cpp \
    $$PWD/utils/Rijndael.cpp \
    $$PWD/sbs2common.cpp \
    $$PWD/sbs2filehandler.cpp \
    $$PWD/sbs2filter.cpp \
    $$PWD/sbs2networkhandler.cpp \
    $$PWD/hardware/sbs2packet.cpp \
    $$PWD/sbs2region.cpp \
    $$PWD/sbs2spectrogram.cpp \
    $$PWD/utils/sbs2timer.cpp \
    $$PWD/sbs2callback.cpp \
    $$PWD/sbs2datahandler.cpp \
    $$PWD/hardware/sbs2datareader.cpp \
    $$PWD/hardware/sbs2hardwaremounter.cpp \
    $$PWD/hardware/emocap/sbs2emocapmounter.cpp \
    $$PWD/hardware/emocap/sbs2emocapdatareader.cpp \
    $$PWD/hardware/emocap/sbs2emocappacket.cpp \
    $$PWD/hardware/emocap28/sbs2emocap28mounter.cpp \
    $$PWD/hardware/emocap28/sbs2emocap28datareader.cpp \
    $$PWD/hardware/emocap28/sbs2emocap28packet.cpp \
    $$PWD/hardware/emotiv/sbs2emotivmounter.cpp \
    $$PWD/hardware/emotiv/sbs2emotivdatareader.cpp \
    $$PWD/hardware/emotiv/sbs2emotivpacket.cpp \
    $$PWD/hardware/filereader/sbs2filedatareader.cpp \
    $$PWD/hardware/filereader/sbs2filepacket.cpp \
    $$PWD/source_reconstruction/sparse/math_utilities.cpp \
    $$PWD/source_reconstruction/loreta/sbs2sourcereconstruction_loreta.cpp \
    $$PWD/source_reconstruction/sparse/sbs2sourcereconstruction_sparse.cpp \
    $$PWD/source_reconstruction/sbs2sourcereconstruction.cpp

HEADERS += \
    $$PWD/FFTReal.h \
    $$PWD/utils/Rijndael.h \
    $$PWD/sbs2common.h \
    $$PWD/sbs2filehandler.h \
    $$PWD/sbs2filter.h \
    $$PWD/sbs2networkhandler.h \
    $$PWD/hardware/sbs2packet.h \
    $$PWD/sbs2region.h \
    $$PWD/sbs2spectrogram.h \
    $$PWD/utils/sbs2timer.h \
    $$PWD/dtu_array_2d.h \
    $$PWD/sbs2callback.h \
    $$PWD/sbs2datahandler.h \
    $$PWD/hardware/sbs2datareader.h \
    $$PWD/jama125/jama_cholesky.h \
    $$PWD/jama125/jama_eig.h \
    $$PWD/jama125/jama_lu.h \
    $$PWD/jama125/jama_qr.h \
    $$PWD/jama125/jama_svd.h \
    $$PWD/jama125/tnt_array1d_utils.h \
    $$PWD/jama125/tnt_array1d.h \
    $$PWD/jama125/tnt_array2d_utils.h \
    $$PWD/jama125/tnt_array2d.h \
    $$PWD/jama125/tnt_array3d_utils.h \
    $$PWD/jama125/tnt_array3d.h \
    $$PWD/jama125/tnt_cmat.h \
    $$PWD/jama125/tnt_fortran_array1d_utils.h \
    $$PWD/jama125/tnt_fortran_array1d.h \
    $$PWD/jama125/tnt_fortran_array2d_utils.h \
    $$PWD/jama125/tnt_fortran_array2d.h \
    $$PWD/jama125/tnt_fortran_array3d_utils.h \
    $$PWD/jama125/tnt_fortran_array3d.h \
    $$PWD/jama125/tnt_i_refvec.h \
    $$PWD/jama125/tnt_math_utils.h \
    $$PWD/jama125/tnt_sparse_matrix_csr.h \
    $$PWD/jama125/tnt_stopwatch.h \
    $$PWD/jama125/tnt_subscript.h \
    $$PWD/jama125/tnt_vec.h \
    $$PWD/jama125/tnt_version.h \
    $$PWD/jama125/tnt.h \
    $$PWD/hardware/sbs2hardwaremounter.h \
    $$PWD/hardware/emocap/sbs2emocapmounter.h \
    $$PWD/hardware/emocap/sbs2emocapdatareader.h \
    $$PWD/hardware/emocap/sbs2emocappacket.h \
    $$PWD/hardware/emocap28/sbs2emocap28mounter.h \
    $$PWD/hardware/emocap28/sbs2emocap28datareader.h \
    $$PWD/hardware/emocap28/sbs2emocap28packet.h \
    $$PWD/hardware/emotiv/sbs2emotivmounter.h \
    $$PWD/hardware/emotiv/sbs2emotivdatareader.h \
    $$PWD/hardware/emotiv/sbs2emotivpacket.h \
    $$PWD/hardware/emotiv/sbs2emotivdecryptor.h \
    $$PWD/hardware/filereader/sbs2filedatareader.h \
    $$PWD/hardware/filereader/sbs2filepacket.h \
    $$PWD/source_reconstruction/sparse/math_utilities.h \
    $$PWD/source_reconstruction/loreta/sbs2sourcereconstruction_loreta.h \
    $$PWD/source_reconstruction/sparse/sbs2sourcereconstruction_sparse.h \
    $$PWD/source_reconstruction/sbs2sourcereconstruction.h

RESOURCES += $$PWD/../sbs2_data/datafiles.qrc
CONFIG += resources_big
