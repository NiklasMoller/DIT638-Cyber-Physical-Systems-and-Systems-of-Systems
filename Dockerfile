##################################################
# Section 1: Build the application
FROM ubuntu:18.04 as builder

# required for wkhtmltodpdf language setting
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

ADD . /opt/sources
WORKDIR /opt/sources

# get argument
ARG arg

RUN ["/bin/bash","-c","cd /opt/sources && \
    #
    # install all dependencies
    bash src/bash-scripts/installer ${arg} && \
    # get all cpp files and store in local variable (! = exclusion)
    ALL_CPP_FILES=$( find . -type f -name '*.cpp' ! -path '*/tests/*' ! -path '*/assets/*') && \
    # 
    # run flawfinder
    bash src/bash-scripts/flawfinder ${arg} $ALL_CPP_FILES  && \ 
    #
    # run scan-build
    bash src/bash-scripts/scan-build ${arg} $ALL_CPP_FILES  && \
    #
    # run pmccabe
    bash src/bash-scripts/pmccabe ${arg} $ALL_CPP_FILES && \
    #
    # run build and test
    bash src/bash-scripts/build-and-test ${arg} && \
    #
    # run coverage test
    bash src/bash-scripts/coverage ${arg} && \
    #
    # create coverage report pdf
    bash src/bash-scripts/pdf-creator ${arg}"]

#################################################
# Section 2: Bundle the application.

# Second stage for packaging the software into a software bundle:
FROM ubuntu:18.04
MAINTAINER Christian Berger "christian.berger@gu.se"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
    libopencv-core3.2 \
    libopencv-highgui3.2 \
    libopencv-imgproc3.2 

WORKDIR /usr/bin

COPY --from=builder /tmp/bin/steering-decider .
COPY --from=builder /tmp/coverage/code_coverage.pdf .
# This is the entrypoint when starting the Docker container; hence, this Docker image is automatically starting our software on its creation
ENTRYPOINT ["/usr/bin/steering-decider"]
