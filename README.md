# BrainGenix API Endpoint Repo

This repository is for the BG API endpoint. The API endpoint executable provides a gateway between users and the raw BG systems (NES/BG/STS).

API specification is located in Docs.


# Building
We use vcpkg for this project, so it's super easy.

All you need to do is cd into the Tools directory, then run `./Install.sh` first to setup 


# Using With Let's Encrypt/Certbot
To enable HTTPS/SSL Support, it's pretty easy. All you'll need to do, is first set this up in HTTP mode, not yet enabling HTTPS. We reccomend using a packaged (.deb) version, which you can create by first Building (Tools/Build.sh), and then packaging (Tools/Package.sh), which will create a debian package in the Artifacts directory. After installing this, edit the config file in /etc/BrainGenix/API/API.cfg to your needs (port wise, DO NOT YET ENABLE HTTPS).

Now, use certbot with a webroot path of `/`, as we serve `/.well-known/acme-challenge/*`. Please note that you'll need root permissions to do this.

Once certbot generates it's certs, and verifies your ownership of whatever domain you use, then simply edit the config file to point to the files that Certbot generates. Please also note that the server will not run if you give the wrong file paths, please use `systemctl status BrainGenix-API.service` to check how your server is running, and adjust config as needed. Use `sudo systemctl restart BrainGenix-API.service` to restart this service whenever you change the config file.

So at this point, you should have your config file point to the SSL cert and privatekey - now just set `UseHTTPS=false` to `UseHTTPS=true` and you should be good to go after restarting the service.

Please note that all traffic will be automatically redirected to HTTPS, so make sure that your users are aware of this.

# Running locally (e.g. to debug NES)

1. Download the repo (or pull if already downloaded to ensure it is up to date).
2. In ./Tools, build with ./Build (use ./Build 4 or so if the machine has limited threads or RAM).
3. In ./Tools, use ./Run.sh to start the API.

(If you are debugging NES then do the same thing with that repo.)

For your client code, install the Python client with pip3 install pyBrainGenixClient,
and in your code use host "localhost" and port "80" and configure UseHTTPS=False when
instantiating the client object.

Your code should now be able to communicate with the local NES and API servers.
