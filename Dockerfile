FROM ubuntu:latest

# Update and install requirements/distro provided steg tools
# * outguess
# * snow
# * stegosuite
# * steghide
RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y \
	git ruby build-essential autotools-dev automake \
	outguess stegsnow stegosuite steghide

# Install zsteg gem
RUN gem install zsteg

# Install stegdetect
RUN git clone https://github.com/abeluck/stegdetect.git && cd stegdetect && sed -i 's/1\.15/1\.16/g' aclocal.m4 configure && mv configure.in configure.ac && mkdir /usr/local/man/man1 && aclocal && automake --add-missing && ./configure && make && make install

# Install jphs (jphide/jpseek/jpcrack)
ADD patches /patches
RUN git clone https://github.com/h3xx/jphs.git&& cd jphs/jpeg-8a && ./configure && make all && cd .. && patch Makefile /patches/jphs-makefile && patch jpseek.c /patches/jphs-jpseek.c && cp jpseek.c jpcrack.c && patch jpcrack.c /patches/jphs-jpcrack.c && patch Makefile /patches/jphs-makefile2 && make && make install && rm -rf /patches

# Set up non-root user
RUN useradd nonroot
USER nonroot

