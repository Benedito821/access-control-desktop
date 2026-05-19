# About
This is a cross-platform desktop app designed to interact with the [access-control](https://https://github.com/Benedito821/access-control) firmware. The intent is to avoid launching the admin.html and the node.js server separately to visualize the users in real time.The present app allows the administrator to launch a single .appimage file to get the server and the admin UI running.

# How to build the .appimage on Ubuntu 

Make sure Docker is installed and running: 

1.  Install Docker: 

```
sudo apt update
sudo apt install docker.io 
```

2. Run the daemon: 
```
sudo systemctl enable --now docker
```

3. Run the builder script(which runs the dockerfile and extracts the appimage artifact from the container):
```
chmod +x build_appimage.sh
./build_appimage.sh
```

The resulting file can be found in the dist/. Appimage Launcher may be used to install the app system wide.
For that user should download and install the package corresponding to their PC architecture from it's [repo](https://github.com/TheAssassin/AppImageLauncher/releases/tag/continuous). 

During development, folders *uploads* and *database* should be copied to the build artifacts folder(e.g. build/Desktop-Debug) in order for the photos and database be found by *QCoreApplication::applicationDirPath()*.
Also make sure the respective folders in project root and in build are in sync.
