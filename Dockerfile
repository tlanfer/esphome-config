FROM python:3

RUN apt-get update
RUN apt-get install -y jq

RUN pip3 install wheel
RUN pip3 install esphome
RUN pip3 install jq yq

ADD build.sh /build.sh
RUN chmod +x /build.sh

WORKDIR /work
ENTRYPOINT [ "/build.sh" ]