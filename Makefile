DEVICES := $(basename $(wildcard *.dev.yaml))

all: $(DEVICES)
	@echo Build for $(DEVICES)

$(DEVICES):
	$(eval DEV=$(shell yq ".ip" $@.yaml))
	@echo esphome --device $(DEV) $@.yaml


.PHONY:	all $(DEVICES)