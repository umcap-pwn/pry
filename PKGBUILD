pkgname=pry
pkgver=0.1.0
pkgrel=1
pkgdesc="ELF static analysis aggregator (hexdump/elf/checksec/disasm)"
arch=('x86_64')
url='https://github.com/umcap-pwn/pry'
license=('MIT')
depends=('glibc' 'capstone')
makedepends=('git')
source=("git+${url}.git#tag=v${pkgver}")
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgname-$pkgver"
  make PREFIX=/usr
}

# check() {
#  cd "$srcdir/$pkgname-$pkgver"
#  make test
#}

package() {
  cd "$srcdir/$pkgname-$pkgver"
  make PREFIX=/usr DESTDIR="$pkgdir" install
}
