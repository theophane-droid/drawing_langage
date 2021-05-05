# Setup
To setup this project first install docker : https://docs.docker.com/get-docker/. Then run docker.
You can now run the following command at the root of this project:

`docker build -t draw_langage_env env`

`docker run --name draw_langage -v "$(pwd):/root" draw_langage_env`

# Remove
Run : `docker rm draw_langage`