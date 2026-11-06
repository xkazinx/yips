#!/usr/bin/env bash
# Installs yips' web build on the Ubuntu box from an uploaded folder (deploy_ubuntu.ps1
# sends it to /tmp/yips-upload): the files under /opt/yips/web, Caddy's snippet in
# /etc/caddy/sites/kazin.dev.d/ (Caddy restarted only when the snippet changed: its
# admin API is off). No service: yips is single player, the page is static.
#   sudo bash install.sh [<upload folder>]
set -euo pipefail
SRC="${1:-/tmp/yips-upload}"
SNIPPET=/etc/caddy/sites/kazin.dev.d/yips.caddy

install -d -m 755 /opt/yips

# The web build swapped whole (a visitor never gets half of two builds)
rm -rf /opt/yips/web.new
cp -r "$SRC/web" /opt/yips/web.new
chmod -R a+rX /opt/yips/web.new
rm -rf /opt/yips/web.old
[ -d /opt/yips/web ] && mv /opt/yips/web /opt/yips/web.old
mv /opt/yips/web.new /opt/yips/web
rm -rf /opt/yips/web.old

if ! cmp -s "$SRC/yips.caddy" "$SNIPPET"; then
	[ -f "$SNIPPET" ] && cp "$SNIPPET" "$SNIPPET.bak"
	install -m 644 "$SRC/yips.caddy" "$SNIPPET"
	if ! caddy validate --config /etc/caddy/Caddyfile --adapter caddyfile >/tmp/yips-caddy.log 2>&1; then
		echo "(!) Caddy refused the snippet:"; tail -5 /tmp/yips-caddy.log
		if [ -f "$SNIPPET.bak" ]; then mv -f "$SNIPPET.bak" "$SNIPPET"; else rm -f "$SNIPPET"; fi
		exit 1
	fi
	rm -f "$SNIPPET.bak"
	systemctl restart caddy
	echo "Caddy: snippet installed, restarted"
fi

ls -l /opt/yips/web
