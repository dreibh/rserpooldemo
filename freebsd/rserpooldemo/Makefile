# Created by: Thomas Dreibholz <thomas.dreibholz@gmail.com>

PORTNAME=	rserpooldemo
DISTVERSION=	3.2.5~rc1.1
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

.include <bsd.port.mk>
