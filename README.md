# Qkinz [![Build Status](https://travis-ci.org/vetlewi/Qkinz.svg?branch=master)](https://travis-ci.org/vetlewi/Qkinz)
Application for simulating the kinematics in Oslo Cyclotron Laboratory. The application uses stopping powers from J.F. Ziegler, J.P. Biersack, and U. Littmark, *The Stopping and Range of Ions in Solids*, Pergamon Press, New York (1985) for materials $Z\leq92$. Stopping powers are calculated from the Bethe-Block formula when $Z>92$. Masses used in the calculations are taken from the Atomic Mass Evaluation 2012 by M. Wang, G. Audi, A. H. Wapstra, F. G. Kondev,M. MacCormick, X. Xu, and B. Pfeiffer, Chinese Physics **C36**, 1603 (2012) [doi:10.1088/1674-1137/36/12/003](dx.doi.org/10.1088/1674-1137/36/12/003). Parts of the graphical component of the software is based on [**QCustomPlot**](http://qcustomplot.com/index.php/introduction).

![Screenshot](https://raw.githubusercontent.com/oslocyclotronlab/Qkinz/master/screenshot/Screenshot.png)

Build
----
The software is based on the [**Qt SDK**](http://www.qt.io/). Currently it has only been built using Qt 5.5 on OS X, but might also work with other versions of Qt. It also requires an compiler supporting C++11. In order to build, run:

`qmake Qkinz.pro`

`make`

Download
----
####OS X:
The latest version for OS X can be [**downloaded here**](https://github.com/vetlewi/Qkinz/releases)(compatible with OS X 10.9 and later).

####Ubuntu:
The easiest way to install for ubuntu is to run the following lines in the command-line:

`sudo add-apt-repository ppa:v-w-ingeberg/ppa`

`sudo apt-get update`

`sudo apt-get install qkinz`

Binaries can also be downloaded from [**here**](https://launchpad.net/~v-w-ingeberg/+archive/ubuntu/ppa/+packages).

####Windows:
Currently, no binaries are provided but can be build from source.

Licence
----
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
