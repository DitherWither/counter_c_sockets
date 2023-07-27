# Counter web application in c with 0 dependencies

This counter web app only depends on glibc/musl at build time, and has zero dependencies
at runtime (other than the kernel if that counts). 

## Building locally

Install the clang compiler, make, and install the static version of the libc

on fedora, this is done with

```bash
sudo dnf in glibc-static make clang
```

on alpine, this is done with

```bash
apk update && apk add build-base musl-dev clang
```

then build the application using

```bash
make all
```

## Building the docker container

The sources come with a dockerfile that can be used to build a container
that uses a `scratch` image as its base, so that the final image is as small
as possible. Alpine is used for the build step

```bash
docker build . --tag="counter"
```

## Running the container

The program is ipv6 only, and needs ipv6 enabled on docker. 
see [the docker docs](https://docs.docker.com/config/daemon/ipv6/) for more info.

Then, run the container using the ipv6 subnet as shown by the guide:
```bash
docker run --rm --network ip6net -p 8080:8080 counter
```
