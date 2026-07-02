# Created by: Thomas Dreibholz <thomas.dreibholz@gmail.com>

PORTNAME=	rserpooldemo
DISTVERSION=	3.2.8
CATEGORIES=	net
MASTER_SITES=	https://www.nntb.no/~dreibh/rserpool/download/

MAINTAINER=	thomas.dreibholz@gmail.com
COMMENT=	RSerPool Demo Tool
WWW=		https://www.nntb.no/~dreibh/rserpool/

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	bash:shells/bash \
		gimp:graphics/gimp3-app \
		gm:graphics/GraphicsMagick \
		exiftool:graphics/p5-Image-ExifTool \
		getopt>=0:misc/getopt \
		noto-sans>=0:x11-fonts/noto-sans
RUN_DEPENDS=	getopt>=0:misc/getopt \
		fractgen:graphics/fractgen \
		rspserver:net/rsplib

USES=		cmake desktop-file-utils gl qt:6 \
		shared-mime-info tar:xz
USE_GL=		gl opengl
USE_QT=         base

CMAKE_ARGS+=	-DCMAKE_INSTALL_MANDIR=${PREFIX}/share/man

MAKE_ENV+=	PYTHONDONTWRITEBYTECODE=1 \
		FONTCONFIG_FILE=${WRKDIR}/fonts.conf

pre-configure:
	@${ECHO_CMD} '<?xml version="1.0"?>' > ${WRKDIR}/fonts.conf
	@${ECHO_CMD} '<!DOCTYPE fontconfig SYSTEM "fonts.dtd">' >> ${WRKDIR}/fonts.conf
	@${ECHO_CMD} '<fontconfig>' >> ${WRKDIR}/fonts.conf
	@${ECHO_CMD} '  <cachedir>${WRKDIR}/.cache/fontconfig</cachedir>' >> ${WRKDIR}/fonts.conf
	@${ECHO_CMD} '  <include>${LOCALBASE}/etc/fonts/fonts.conf</include>' >> ${WRKDIR}/fonts.conf
	@${ECHO_CMD} '</fontconfig>' >> ${WRKDIR}/fonts.conf

.include <bsd.port.mk>
