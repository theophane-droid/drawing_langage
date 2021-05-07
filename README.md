# 1. Installation
# 1.1 Setup with docker
To setup this project first install docker : https://docs.docker.com/get-docker/. Then run docker.
You can now run the following command at the root of this project:

`docker build -t draw_langage_env env`

`docker run --name draw_langage -v "$(pwd):/root" draw_langage_env`

# 1.2 Setup on linux distro
Ensure to have gcc + make installed

# 2 Compile the project
Just run

`make` 

at the root of the project. Greetings ! You can now run the project by running 

`./build/draw /path/to/filetoexecute`

# 3 Use the drawing language

There is an example of a working program in the folder example. There is no loop instructions, you can use if boxes for that.
