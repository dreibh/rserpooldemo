# Created by: Thomas Dreibholz <thomas.dreibholz@gmail.com>

PORTNAME=	rserpooldemo
PORTVERSION=	3.1.3
CATEGORIES=	net
MASTER_SITES=	https://www.nntb.no/~dreibh/rserpool/download/

MAINTAINER=	thomas.dreibholz@gmail.com
COMMENT=	RSerPool Demo Tool
WWW=		https://www.nntb.no/~dreibh/rserpool/

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	gimp:graphics/gimp-app \
		gm:graphics/GraphicsMagick \
		inkscape:graphics/inkscape \
		exiftool:graphics/p5-Image-ExifTool
RUN_DEPENDS=    rspserver:net/rsplib \
		fractgen:graphics/fractgen

USES=		cmake qt:5 shebangfix tar:xz

CMAKE_ARGS+=	-DCMAKE_INSTALL_MANDIR=${PREFIX}/share/man

USE_LDCONFIG=	yes
USE_QT=		buildtools_build core gui qmake widgets xml

.include <bsd.port.mk>
