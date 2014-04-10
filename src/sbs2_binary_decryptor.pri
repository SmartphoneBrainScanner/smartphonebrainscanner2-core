QT += network

macx {
    LIBS += -framework IOKit -framework CoreFoundation
    SOURCES += $$PWD/platform/osx/hid.c
    HEADERS +=  $$PWD/platform/osx/hidapi.h
    message("OSX")
}

unix:!macx:!android-g++ {
    LIBS += `pkg-config libudev --libs` -lrt
    SOURCES += $$PWD/platform/linux/hid.c
    HEADERS +=  $$PWD/platform/linux/hidapi.h
    message("LINUX")
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
    $$PWD/source_reconstruction/sparse/math_utilities.cpp \
    $$PWD/source_reconstruction/sparse/sbs2sourcereconstruction_sparse.cpp \
    $$PWD/source_reconstruction/loreta/sbs2sourcereconstruction_loreta.cpp \
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
    $$PWD/utils/waiter.h \
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
    $$PWD/source_reconstruction/sparse/math_utilities.h \
    $$PWD/source_reconstruction/sparse/sbs2sourcereconstruction_sparse.h \
    $$PWD/source_reconstruction/loreta/sbs2sourcereconstruction_loreta.h \
    $$PWD/source_reconstruction/sbs2sourcereconstruction.h
