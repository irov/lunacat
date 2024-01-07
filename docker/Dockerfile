## build lunacat-bin
FROM debian:12 as builder

ENV DEBUG=True

## install dependencies
RUN apt-get update && apt-get install -y \
  build-essential \
  libsnappy-dev \
  libssl-dev \
  libz-dev \
  openssl \
  python3 \
  cmake \
  git 

## optional for debugging 
# RUN apt-get install -y tree

## hb_grid build, 
## copy files - because developer has to define build version
WORKDIR /app

COPY . .

WORKDIR /app/build/downloads/
RUN bash ./downloads_unix.bash

WORKDIR /app/build/unix/
RUN bash ./build_dependencies_debug.bash && bash ./build_solution_debug.bash

# RUN rm -rf /app/
# create runtime for lunacat and run
FROM debian:12

LABEL org.opencontainers.image.title = "Lunacat"

RUN apg-get update
# RUN apt-get install -y net-tools \
#   iputils-ping \
#   iproute2 \ 
#   strace \
#   gnupg \
#   wget 

WORKDIR /app
## TODO: updated path
COPY --from=builder /app/path/to/bin /app
EXPOSE 5555
RUN chmod +X ./hb_grid ./docker-entrypoint.sh
## TODO: check debug.json path
CMD ["hb_grid", "--config ./debug.json"]
