FROM nordicplayground/nrfconnect-sdk:main

SHELL ["nrfutil","toolchain-manager","launch","/bin/bash","--","-c"]
RUN apt-get update && apt-get upgrade -y; apt-get install --no-install-recommends -y \
  clangd ssh nano bash-completion sudo gpg ruby lcov screen libfftw3-dev curl python3-venv git-lfs golang-go

RUN go install github.com/apache/mynewt-mcumgr-cli/mcumgr@latest
RUN cp /root/go/bin/* /usr/local/bin/

RUN cd /workdir; west update

# add your commands below
