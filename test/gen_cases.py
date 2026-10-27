"""Erzeugt Referenzwerte mit Python (zoneinfo/urllib) fuer die C++-Tests."""
import random, sys
from datetime import datetime, timedelta, timezone
from urllib.parse import quote, unquote_plus
from zoneinfo import ZoneInfo
B = ZoneInfo("Europe/Berlin")
out = open(sys.argv[1], "w")
random.seed(1)
start = datetime(2024, 1, 1, tzinfo=timezone.utc)
for i in range(6000):
    dt = start + timedelta(minutes=random.randrange(0, 5 * 365 * 24 * 60))
    for iso in (dt.astimezone(B).isoformat(timespec="seconds"),
                dt.astimezone(B).isoformat(timespec="milliseconds"),
                dt.strftime("%Y-%m-%dT%H:%M:%SZ"),
                dt.strftime("%Y-%m-%dT%H:%M:%S+0000")):
        out.write("ISO\t%s\t%d\n" % (iso, int(dt.timestamp())))
for iso, ts in (("2026-10-03T05:25:00+05:30", None), ("2026-10-02T21:55:00-02:00", None)):
    out.write("ISO\t%s\t%d\n" % (iso, int(datetime(2026, 10, 2, 23, 55, tzinfo=timezone.utc).timestamp())))
for s in ["U Yorckstraße", "S+U Alexanderplatz", "Café Müller/Ä Ö Ü ß", "a&b=c?d#e", "~-_.", "日本"]:
    out.write("ENC\t%s\t%s\n" % (s, quote(s, safe="-_.~")))
for s in ["Yorckstra%C3%9Fe", "S%2BU+Alexanderplatz", "a%20b", "K%C3%B6penick", "100%", "%zz", "%2", "plain",
          "%E6%97%A5%E6%9C%AC", "a%26b%3Dc", "+", "%25"]:
    out.write("DEC\t%s\t%s\n" % (s, unquote_plus(s)))
