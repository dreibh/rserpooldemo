pdffilesfromfig := $(patsubst %.fig,%.pdf,$(wildcard *.fig))
pdffilesfromdia := $(patsubst %.dia,%.pdf,$(wildcard *.dia))
pngfiles        := $(patsubst %.pdf,%.png,$(wildcard *.pdf))


all:   $(pdffilesfromfig) $(pdffilesfromdia) $(pngfiles)



$(pdffilesfromfig):	%.pdf: %.fig
	fig2dev -L pdf -p dummy $< $@

$(pdffilesfromdia):	%.pdf: %.dia
	dia -e tmp.eps $<
	epstopdf --outfile=$@ tmp.eps
	rm -f tmp.eps

$(pngfiles):	%.png: %.pdf
	convert -density 180x180 $< -transparent "#ff00ff" $@
