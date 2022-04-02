FROM python:3

RUN apt-get update
RUN apt-get install -y jq

RUN pip3 install wheel
RUN pip3 install esphome
RUN pip3 install jq yq

WORKDIR /work
CMD make