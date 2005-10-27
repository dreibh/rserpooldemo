pngfiles        := $(patsubst %.dia,%.png,$(wildcard *.dia))

all:   $(pngfiles)

$(pngfiles):	%.png: %.dia
	dia -e tmp.png $< -s 85
	convert tmp.png -transparent "#ff00ff" $@
	rm -f tmp.png
