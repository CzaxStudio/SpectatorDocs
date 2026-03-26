# Spectator — Security Modules

> Built-in security modules. No imports required — available in every script.

---

## Table of Contents

- [Recon](#recon)
- [DNS](#dns)
- [Port Scanning](#port-scanning)
- [OSINT](#osint)
- [Web Analysis](#web-analysis)
- [Exploitation Modules](#exploitation-modules)
- [Payload Generator](#payload-generator)
- [Encoding & Decoding](#encoding--decoding)
- [Hashing & Cracking](#hashing--cracking)
- [Secret Scanning](#secret-scanning)
- [Mission Engine](#mission-engine)
- [TUI Display](#tui-display)

---

## Recon

Full reconnaissance on a target — DNS, MX, NS, TXT, CNAME, banner grab.

```spectator
do --> Recon("target.com")
```

**Output includes:** IP addresses, MX records, NS records, TXT records, CNAME.

### Individual DNS Functions

```spectator
ips   = resolve("target.com")           ## A records → list of IPs
mxs   = lookupMX("target.com")          ## MX records → list of hostnames
nss   = lookupNS("target.com")          ## NS records → list of nameservers
txts  = lookupTXT("target.com")         ## TXT records → list of strings
cname = lookupCNAME("target.com")       ## CNAME → string
rdns  = lookupAddr("93.184.216.34")     ## Reverse DNS → list of hostnames
```

### Banner Grabbing

```spectator
do --> BannerGrab("target.com", 22)   ## grab banner from port 22
do --> BannerGrab("target.com", 80)
```

### Ping Check

```spectator
alive = PingCheck("target.com")
Trace(str(alive))   ## true / false
```

### CIDR Scanning

```spectator
do --> CIDRScan("192.168.1.0/24")     ## scan entire subnet
hosts = cidrHosts("192.168.1.0/30")   ## get list of IPs in range
```

### Concurrent Host Scan

```spectator
hosts = ["target1.com", "target2.com", "192.168.1.1"]
do --> ConcurrentScan(hosts)
```

### OS Info

```spectator
do --> OSInfo()   ## current machine OS, hostname, CPUs, interfaces
```

---

## DNS

```spectator
## Full DNS lookup module
do --> DNSLookup("target.com")

## Subdomain enumeration — checks 49 common subdomains via DNS
do --> SubdomainEnum("target.com")

## Subdomain takeover check
do --> SubTakeover("target.com")
```

---

## Port Scanning

```spectator
## Scan a range of ports
do --> PortScan("target.com", 1, 1024)
do --> PortScan("target.com", 1, 65535)

## Check a single port
if hasPort("target.com", 443) {
  Trace("Port 443 is open")
}

## Port service map — returns map of port→service name
svcs = portServices()
Trace(svcs["22"])     ## "SSH"
Trace(svcs["80"])     ## "HTTP"
Trace(svcs["3306"])   ## "MySQL"
```

### Common Ports Reference

| Port | Service |
|------|---------|
| 21 | FTP |
| 22 | SSH |
| 23 | Telnet |
| 25 | SMTP |
| 53 | DNS |
| 80 | HTTP |
| 110 | POP3 |
| 143 | IMAP |
| 443 | HTTPS |
| 445 | SMB |
| 3306 | MySQL |
| 3389 | RDP |
| 5432 | PostgreSQL |
| 6379 | Redis |
| 8080 | HTTP-Alt |
| 8443 | HTTPS-Alt |
| 9200 | Elasticsearch |
| 27017 | MongoDB |

---

## OSINT

### WHOIS

```spectator
do --> WHOIs("target.com")
```

### GeoIP

```spectator
do --> GeoIP("93.184.216.34")   ## country, city, ISP, ASN
do --> IPInfo("93.184.216.34")  ## extended IP intelligence
```

### GeoIP + Threat Intel

```spectator
ips = resolve("target.com")
each ip : ips {
  do --> GeoIP(ip)
  Trace("Shodan : https://www.shodan.io/host/" --> ip)
  Trace("HIBP   : https://www.abuseipdb.com/check/" --> ip)
}
```

---

## Web Analysis

### HTTP Probe

```spectator
do --> HTTPProbe("https://target.com")
```

### Headers

```spectator
do --> Headers("https://target.com")
```

### Security Header Audit

```spectator
do --> HeaderAudit("https://target.com")
```

**Checks for:** HSTS, CSP, X-Frame-Options, X-Content-Type-Options, Referrer-Policy, Permissions-Policy.

### SSL / TLS Info

```spectator
do --> SSLInfo("target.com")
```

**Shows:** certificate issuer, expiry, SANs, TLS version, cipher suite.

### Technology Detection

```spectator
do --> TechDetect("https://target.com")
```

**Detects:** CMS (WordPress, Joomla, Drupal), frameworks (Laravel, Django, Rails), JS libraries (React, Angular, Vue), server software (Nginx, Apache).

### CORS Test

```spectator
do --> CORSTest("https://target.com")
```

### Open Redirect Test

```spectator
do --> OpenRedirect("https://target.com/redirect?url=")
```

### Email Harvest

```spectator
do --> EmailHarvest("https://target.com")
```

### Directory Busting

```spectator
do --> DirBust("https://target.com", wordlist("admin_paths"))
```

### URL Fuzzing

```spectator
do --> FuzzURL("https://target.com/page?id=", ["1","2","admin","../"])
```

### SQL Injection Test

```spectator
do --> SQLiTest("https://target.com/page?id=1")
```

### Command Injection Test

```spectator
do --> CMDInject("https://target.com/ping?host=")
```

### Path Traversal Test

```spectator
do --> PathTraversal("https://target.com/file?name=")
```

---

## Payload Generator

### Using `do -->`

Prints formatted, colored output to terminal:

```spectator
do --> PayloadGen("xss")
do --> PayloadGen("sqli")
do --> PayloadGen("ssrf")
do --> PayloadGen("lfi")
do --> PayloadGen("cmd")
do --> PayloadGen("redirect")
do --> PayloadGen("nosql")
do --> PayloadGen("ldap")
do --> PayloadGen("ssti")
do --> PayloadGen("xxe")
```

### Using `payloadList()` — GUI / Script Friendly

Returns a plain list — no ANSI codes, perfect for GUI output boxes:

```spectator
payloads = payloadList("xss")
each p, idx : payloads {
  Trace("[" --> str(idx+1) --> "] " --> p)
}
```

### Built-in Wordlists

```spectator
wordlist("admin_paths")        ## 24 common admin paths
wordlist("sensitive_files")    ## 40 sensitive file paths
wordlist("common_subdomains")  ## 49 common subdomains
wordlist("api_paths")          ## common API endpoint paths
```

---

## Encoding & Decoding

### Encode

```spectator
Encode("<script>alert(1)</script>", "url")      ## %3Cscript%3Ealert%281%29%3C%2Fscript%3E
Encode("payload", "url2")                       ## double URL encode
Encode("payload", "double_url")                 ## same as url2
Encode("payload", "html")                       ## HTML entities
Encode("payload", "html5")                      ## HTML5 entities
Encode("payload", "hex")                        ## hex encoding
Encode("payload", "b64")                        ## Base64
Encode("payload", "unicode")                    ## Unicode escape
Encode("payload", "rot13")                      ## ROT13
Encode("payload\x00test", "null")               ## null byte injection
```

### Decode

```spectator
Decode("%3Cscript%3E", "url")     ## <script>
Decode("cGF5bG9hZA==", "b64")    ## payload
Decode("&#x3C;", "html")          ## <
Decode("3c736372697074 3e", "hex") ## <script>
Decode("cnlybybq", "rot13")       ## payload
```

### Cipher Solve

Automatically detects and decodes common ciphers:

```spectator
do --> CipherSolve("Uryyb Jbeyq")        ## detects ROT13 → "Hello World"
do --> CipherSolve("SGVsbG8gV29ybGQ=")   ## detects Base64 → "Hello World"
do --> CipherSolve("48656c6c6f")          ## detects Hex → "Hello"
```

---

## Hashing & Cracking

### Identify Hash Type

```spectator
do --> HashIdentify("5f4dcc3b5aa765d61d8327deb882cf99")
## Output: [+] MD5  [+] NTLM  [+] MD4
```

### Crack a Hash

```spectator
do --> Crack("5f4dcc3b5aa765d61d8327deb882cf99", "md5")
## Output: [CRACKED] → password
```

Crack uses a built-in wordlist of common passwords.

### Compute Hashes

```spectator
md5("spectator")
sha1("spectator")
sha256("spectator")
base64enc("spectator")
base64dec("c3BlY3RhdG9y")
```

### Common Passwords

```spectator
passwords = commonPasswords()
Trace(str(len(passwords)) --> " passwords loaded")
```

---

## Secret Scanning

Scans text for leaked credentials, API keys, and secrets:

```spectator
content = readFile("config.env")
do --> SecretScan(content)
```

**Detects:**
- AWS Access Keys (`AKIA...`)
- Generic API keys (`API_KEY=...`)
- Private keys (`-----BEGIN RSA PRIVATE KEY-----`)
- Database connection strings
- Bearer tokens
- GitHub/GitLab tokens
- Slack webhooks
- Google API keys
- JWT tokens

### Scan a URL's Source

```spectator
resp    = http("GET", "https://target.com/app.js", {})
body    = httpBody(resp)
do --> SecretScan(body)
```

---

## Mission Engine

Structure your pentest as a tracked mission with automatic HTML report generation.

### Basic Usage

```spectator
## Start mission
m = missionStart("Web App Pentest", "app.target.com")

## Add a stage
missionStage(m, "Reconnaissance")

## Record findings
missionFind(m, "CRITICAL", "SQL Injection",      "POST /login — username param")
missionFind(m, "HIGH",     "XSS Reflected",      "GET /search?q= — unescaped output")
missionFind(m, "MEDIUM",   "Missing HSTS",       "No Strict-Transport-Security header")
missionFind(m, "LOW",      "Directory Listing",  "Apache /images/ directory exposed")
missionFind(m, "INFO",     "Tech Stack",         "WordPress 6.2 + PHP 8.1 + Nginx")

## Add notes
missionNote(m, "Admin panel found at /wp-admin — not tested")

## Store arbitrary data
missionData(m, "open_ports", [80, 443, 8080])
missionData(m, "subdomains", ["api.target.com", "dev.target.com"])

## Retrieve stored data
ports = missionGet(m, "open_ports")
Trace(str(ports))

## End mission
missionEnd(m)

## Generate HTML report
missionReport(m, "pentest_report.html")
Trace("Report saved.")
```

### Severity Levels

| Level | Use for |
|-------|---------|
| `CRITICAL` | Remote code execution, auth bypass, data breach |
| `HIGH` | SQLi, XSS, IDOR, privilege escalation |
| `MEDIUM` | Missing security headers, info disclosure |
| `LOW` | Best practice violations, minor config issues |
| `INFO` | Notes, tech stack, observations |

### Query Findings

```spectator
## Get all findings
findings = missionFindings(m)
Trace(str(len(findings)) --> " findings")

## Get summary counts
summary = missionSummary(m)
Trace("Critical : " --> str(summary["critical"]))
Trace("High     : " --> str(summary["high"]))
Trace("Medium   : " --> str(summary["medium"]))
Trace("Low      : " --> str(summary["low"]))
Trace("Info     : " --> str(summary["info"]))
```

### Multi-Stage Mission

```spectator
m = missionStart("Full Pentest", "target.com")

missionStage(m, "Recon")
missionFind(m, "INFO", "Subdomains found", "api, dev, staging")
missionNote(m, "Target runs WordPress 6.2")

missionStage(m, "Scanning")
missionFind(m, "MEDIUM", "Port 8080 open", "Tomcat admin interface")

missionStage(m, "Exploitation")
missionFind(m, "CRITICAL", "Tomcat default creds", "admin:admin — full access")

missionStage(m, "Post-Exploitation")
missionFind(m, "HIGH", "Internal network access", "10.0.0.0/8 reachable")

missionEnd(m)
missionReport(m, "full_pentest.html")
```

The HTML report includes:
- Executive summary
- Findings table sorted Critical → Info
- Severity distribution chart
- Stage timeline
- Full notes

---

## TUI Display

Terminal UI utilities for professional output.

```spectator
## ASCII banner
banner("SPECTATOR RECON")

## Colored output
Trace(colorize("CRITICAL finding", "red"))
Trace(colorize("Test passed",      "green"))
Trace(colorize("Warning",          "yellow"))
Trace(colorize("Info",             "cyan"))
Trace(colorize("Detail",           "magenta"))
Trace(colorize("Bold text",        "bold"))
Trace(colorize("Dim text",         "dim"))

## ASCII table
table(
  ["Port", "Service", "Status"],
  [
    ["80",   "HTTP",  "OPEN"],
    ["443",  "HTTPS", "OPEN"],
    ["3306", "MySQL", "OPEN"]
  ]
)

## Progress bar
loop 20 {
  progress(_i + 1, 20, "Scanning ports...")
  sleep(50)
}

## Pipeline utilities
words = ["HIGH","LOW","HIGH","CRITICAL","MEDIUM","HIGH"]
Trace(str(tally(words)))         ## count occurrences
Trace(str(sortList(words)))      ## sort alphabetically
Trace(str(diff(l1, l2)))         ## items in l1 not in l2
Trace(str(intersect(l1, l2)))    ## items in both lists
Trace(str(gather([l1, l2, l3]))) ## flatten list of lists

## Interactive input
target = Capture("Enter target: ")
```
