# Created by: Thomas Dreibholz <dreibh@iem.uni-due.de>
# $FreeBSD: head/net/rsplib/Makefile 509199 2019-08-18 10:42:52Z pi $

PORTNAME=	rserpooldemo
PORTVERSION=	3.0.4
CATEGORIES=	net
MASTER_SITES=	https://www.uni-due.de/~be0001/rserpool/download/

MAINTAINER=	dreibh@iem.uni-due.de
COMMENT=	RSerPool Demo Tool

LICENSE=	GPLv3+
LICENSE_FILE=	${WRKSRC}/COPYING

USES=		cmake qt:5 shebangfix tar:xz
RUN_DEPENDS=    rsplib:net/rsplib \
		fractgen:graphics/fractgen

BUILD_DEPENDS=  gimp:graphics/gimp-app \
                gm:graphics/GraphicsMagick \
                inkscape:graphics/inkscape \
                exiftool:graphics/p5-Image-ExifTool \
                libomp.so:devel/llvm-devel
# Fix for build with FreeBSD 12.0-RELEASE: devel/llvm-devel for libomp required by Gimp!

CMAKE_ARGS+=	-DCMAKE_INSTALL_MANDIR=${PREFIX}/man \
		-DDOCSDIR="${DOCSDIR}"

SHEBANG_FILES=	src/scriptingserviceexample
USE_LDCONFIG=	yes
USE_QT=		buildtools_build core gui qmake widgets xml

.include <bsd.port.mk>
