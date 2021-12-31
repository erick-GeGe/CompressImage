# COMPRESOR DE IMÁGENES USANDO QUAD-TREE
[![Build Status](https://badgen.net/badge/build/passing/green)](https://github.com/erick-GeGe/CompressImage)
[![Versión](https://badgen.net/badge/version/0.9.1/blue)](https://github.com/erick-GeGe/CompressImage)
## PRE-REQUISITOS
### DISTRO
🔵 Linux - Xubuntu
### HAMACHI
  Descargar [instalador](https://www.vpn.net/linux) acorde a la arquitectura del procesador (32 o 64 bits)
  ```bash
cd ~/User/Carpeta_Contenedora_Descarga
sudo dpkg -i logmein-hamachi_2.1.0.203-1_amd64.deb
sudo -i
hamachi login
hamachi attach <tu_email@unsa.edu.pe>
hamachi do-join <Red-Virtual>
hamachi
"Ctrl + D"
  ```
  Configurar los nodos de la red
  ```bash
cd ~
sudo nano /etc/hosts
  ```
  Modificar el archivo hosts con las direcciones ip y nombres de cada una
  ```
  xxx.xxx.xxx.xxx     master
  xxx.xxx.xxx.xxx     worker1
  xxx.xxx.xxx.xxx     worker2
  ```
### SSH (Secure Shell)
Instalar y configurar las dependencias de SSH
```bash
sudo apt-get install openssh-server
mkdir ~/.ssh
chmod 700 ~/.ssh
ssh-keygen -t rsa
/home/mpiu/.ssh/id_rsa_<worker n>
scp id_rsa_<worker n>.pub <tu_usuario>@master:~/.ssh
cat /home/<master_user>/.ssh/id_rsa_master.pub >> /home/<master_user>/.ssh/authorized_keys
cd ~
sudo nano /etc/ssh/sshd_config
```
```
PermitRootLogin yes
RSAAuthentication yes
PubkeyAuthentication yes
PasswordAuthentication yes
```
```bash
sudo service ssh restart
ssh master
```

### NFS (Network File System)
#### Master
```bash
sudo apt-get install nfs-kernel-server
sudo mkdir -p /home/<tu_usuario>/share
sudo chown nobody:nogroup /home/<tu_usuario>/share
sudo chmod 777 /home/<tu_usuario>/share
sudo nano /etc/exports

/home/<tu_usuario>/share worker1(rw,sync,no_subtree_check)
/home/<tu_usuario>/share worker2(rw,sync,no_subtree_check)
sudo exportfs -a
sudo systemctl restart nfs-kernel-server

sudo ufw status

```
#### Worker
```bash
sudo apt-get install nfs-common
sudo mkdir ~/share
cd ~
sudo nano /etc/fstab
master:/home/<tu_usuario>/share home/mpiu/share nfs
Ctrl+O ENTER Ctrl+X

sudo mount -a
sudo ufw status
```
### OpenMPI (Message Passing Interface)
```bash
sudo apt-get update
sudo apt-get install gcc openmpi-bin openmpi-common libopenmpi-dev libgtk2.0-dev 
```
Descargar la [carpeta comprimida](https://www.open-mpi.org/software/ompi/v4.1/) y después de la extracción
```bash
cd ~/User/Carpeta_Contenedora_Descarga/openmpi-<version_descargada>/
./configure --prefix="/home/<tu_usuario>/.openmpi"
make
sudo make install
ahora exportar el PATH
export PATH="$PATH:/home/<tu_usuario>/.openmpi/bin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/<tu_usuario>/.openmpi/lib"
```
### OPENCV para C++
Actualizar las dependencias previas
```bash
sudo apt-get update && sudo apt-get upgrade
sudo apt install software-properties-common apt-file
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-add-repository ppa:deadsnakes/ppa
sudo apt-get install libtbb2 libtbb-dev

sudo apt-add-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt update
sudo apt install libjasper1 libjasper-dev
```
Instalar OpenCV
```bash
sudo -s
cd ~ && cd /opt
git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git
cd opencv
mkdir release
cd release
cmake -D BUILD_TIFF=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D OPENCV_GENERATE_PKGCONFIG=ON -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
make -j4
make install
ldconfig
exit

cd ~
apt-file search opencv.pc
ls /usr/local/lib/pkgconfig/
sudo cp /usr/local/lib/pkgconfig/opencv4.pc  /usr/lib/x86_64-linux-gnu/pkgconfig/opencv.pc
pkg-config --modversion opencv
```

## Compilacion y ejecucion
### Compilar con makefile
```
make
mkdir compressed_image
mkdir test
```
### Ejecutar 
```
mpirun -host master,worker1,worker2 ./compress_image <src de la imagen> <threshold>
```
Ejemplo
```
mpirun -host master,worker1,worker2 ./compress_image test01.jpg 5
```
Esto comprimira la imagen de manera distribuida en 16 pequeñas partes para juntarlas se debe correl el otro programa, el cual juntara la imagen en una sola y la guarda en la carpeta test
```
./join_image
```

