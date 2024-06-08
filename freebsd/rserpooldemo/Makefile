# Created by: Thomas Dreibholz <thomas.dreibholz@gmail.com>

PORTNAME=	rserpooldemo
DISTVERSION=	3.1.6
CATEGORIES=	net
MASTER_SITES=	https://www.nntb.no/~dreibh/rserpool/download/

MAINTAINER=	thomas.dreibholz@gmail.com
COMMENT=	RSerPool Demo Tool
WWW=		https://www.nntb.no/~dreibh/rserpool/

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	gimp:graphics/gimp-app \
		gm:graphics/GraphicsMagick \
		exiftool:graphics/p5-Image-ExifTool
RUN_DEPENDS=    rspserver:net/rsplib \
		fractgen:graphics/fractgen

USES=		cmake desktop-file-utils shared-mime-info qt:5 tar:xz

CMAKE_ARGS+=	-DCMAKE_INSTALL_MANDIR=${PREFIX}/share/man

USE_LDCONFIG=	yes
USE_QT=         buildtools:build core gui network widgets xml

.include <bsd.port.mk>
