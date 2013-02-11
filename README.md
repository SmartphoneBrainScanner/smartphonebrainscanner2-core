## ![logo.png](https://raw.github.com/wiki/SmartphoneBrainScanner/smartphonebrainscanner2-core/Images/logo.png) smartphonebrainscanner2

SmartphoneBrainScanner2 is a framework for building cross-platform real-time EEG applications. Originally developed at Technical University of Denmark for collecting and analyzing signals from Emotiv EPOC headset, its extensible architecture allows working with various EEG systems and multiple platforms.

# [Get Started →](https://github.com/SmartphoneBrainScanner/smartphonebrainscanner2-core/wiki/Getting-Started)

![brain_background.jpg](https://raw.github.com/wiki/SmartphoneBrainScanner/smartphonebrainscanner2-core/Images/brain_background.jpg)

# Cross Platform
SmartphoneBrainScanner2 is written in [Qt](http://qt.digia.com/), a C++ framework offering the power of the native development and unified support for multiple platforms. Plus the UI can be created in QML, high-level declarative UI framework. 
SBS2 can be compiled for every platform supporting Qt 4, including Linux, OSX, Windows, Android, Maemo 5, MeeGo. Although not yet attempted, it should also work on iOS and BlackBerry OS.

# Advanced EEG
SmartphoneBrainScanner2 contains state-of-the-art techniques for working with multi-channel EEG signal in real-time, most notably source reconstruction methods with online adaptation to the noise level. Current implemented source reconstruction approaches cover the minimum-norm and low resolution tomography (LORETA) methods formulated in a Bayesian framework using a expectation-maximization scheme for hyperparameter estimation.
The SBS2 source reconstruction is realized using a pre-build forward model connecting the cortical surface with the electrodes at the scalp. The current forward model provided with the software is a 3-spheres model obtained from the Matlab toolbox SPM8 using coarse spatial resolution and with sensor positions in accordance with the Emotiv EPOC system.

Besides, source reconstruction methods, additional machine learning methods such as independent component analysis (ICA), common spatial patterns (CSP), and Bayesian classifiers are continuously added. 

# New Approach
Real-time EEG doesn't have to happen in the lab! Consumer-grade and inexpensive research neuroheadsets allow for portability, delivering high-quality EEG signal. SmartphoneBrainScanner2 apps can be developed just like any other apps, featuring reach interface, connectivity, etc. Go, create!

# How to cite SBS2
Please acknowledge the work of the SmartphoneBrainScanner2? by citing (Stopczynski et al, 2011). Additional project related references can be found [here](https://github.com/SmartphoneBrainScanner/smartphonebrainscanner2-core/wiki/References).

A. Stopczynski, J. E. Larsen, C. Stahlhut, M. K. Petersen, & L. K. Hansen (2011), _A smartphone interface for a wireless EEG headset with real-time 3D reconstruction_, Affective Computing and Intelligent Interaction (ACII 2011), Lecture Notes in Computer Science (LNCS) 6357, Springer-Verlag Berlin Heidelberg, pp.317-318.


    @article{stopczynski2011smartphone,
      title={A smartphone interface for a wireless EEG headset with real-time 3D reconstruction},
      author={Stopczynski, A. and Larsen, J.E. and Stahlhut, C. and Petersen, M.K. and Hansen, L.K.},
      journal={Affective Computing and Intelligent Interaction},
      pages={317--318},
      year={2011},
      publisher={Springer}
    }

# Developer team
* Arkadiusz Stopczynski, DTU Compute
* Carsten Stahlhut, DTU Compute
* Michael Kai Petersen, DTU Compute
* Jakob Eg Larsen, DTU Compute
* Lars Kai Hansen, DTU Compute

# [Contact →](https://github.com/SmartphoneBrainScanner/smartphonebrainscanner2-core/wiki/Contact)


# Acknowledgement
This work is supported in part by
* [Danish Lundbeck Foundation](http://www.lundbeckfonden.dk/Frontpage.20.aspx) through Center for Integrated Molecular Brain Imaging ([CIMBI](http://www.cimbi.dk/))
* [Danish Lundbeck Foundation](http://www.lundbeckfonden.dk/Frontpage.20.aspx) through the project Real-time brain imaging by EEG
* H.C. Ørsteds Foundation
* [Nokia](http://www.nokia.com) (we thank Nokia for partly sponsoring hardware for the project) 

![acknowledgement.png](https://raw.github.com/wiki/SmartphoneBrainScanner/smartphonebrainscanner2-core/Images/acknowledgement.png)
