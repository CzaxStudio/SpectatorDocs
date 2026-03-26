# Spectator — Language Syntax

> The complete syntax reference for Spectator v2.0.0

---

## Table of Contents

- [Comments](#comments)
- [Variables](#variables)
- [Data Types](#data-types)
- [Strings](#strings)
- [Operators](#operators)
- [Collections](#collections)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Error Handling](#error-handling)
- [Concurrency](#concurrency)
- [Shell Execution](#shell-execution)
- [Imports](#imports)
- [Built-in Helpers](#built-in-helpers)

---

## Comments

```spectator
## This is a comment
## Comments start with ##
```

---

## Variables

No type declarations. Variables are dynamically typed.

```spectator
x     = 42
name  = "Spectator"
flag  = true
price = 3.14
empty = nil

## let is optional — identical to plain assignment
let version = "2.0.0"
```

### Augmented Assignment

```spectator
x += 10
x -= 5
x *= 2
x /= 4
```

---

## Data Types

| Type | Example |
|------|---------|
| Integer | `42` |
| Float | `3.14` |
| String | `"hello"` |
| Boolean | `true` / `false` |
| Nil | `nil` |
| List | `[1, 2, 3]` |
| Map | `{"key": "value"}` |

### Type Conversion

```spectator
str(42)          ## "42"
num("3.14")      ## 3.14
bool(1)          ## true
bool(0)          ## false
bool("")         ## false
```

---

## Strings

### Basic Strings

```spectator
name = "Spectator"
path = "C:\\Users\\Ghost"
```

### Concatenation

Use `-->` to join values of any type:

```spectator
msg = "Hello " --> name --> "!"
msg = "Port " --> str(80) --> " is open"
```

### f-Strings

Embed variables directly with `f"..."`:

```spectator
host = "target.com"
port = 443
msg  = f"Scanning {host} on port {port}"
Trace(msg)
```

> **Note:** f-strings support simple variable names. For computed values assign to a variable first.

### String Functions

```spectator
upper("hello")              ## "HELLO"
lower("WORLD")              ## "world"
trim("  hello  ")           ## "hello"
reverse("abc")              ## "cba"
repeat("*", 10)             ## "**********"
len("spectator")            ## 9

replace("hello world", "world", "spectator")  ## "hello spectator"
contains("spectator", "spec")                 ## true
startsWith("spectator", "spec")               ## true
endsWith("spectator", "tor")                  ## true

split("a,b,c", ",")         ## ["a", "b", "c"]
join(["a","b","c"], "-")    ## "a-b-c"
substr("spectator", 0, 5)   ## "spect"
truncate("long string", 8)  ## "long ..."
pad("hello", 10)            ## "hello     "

## Regex
regex("[0-9]+", "port 443 open")   ## ["443"]
regexMatch("^\\d+$", "12345")      ## true
```

### Hashing

```spectator
md5("spectator")
sha1("spectator")
sha256("spectator")
base64enc("spectator")
base64dec("c3BlY3RhdG9y")
```

---

## Operators

### Arithmetic

```spectator
10 + 3    ## 13
10 - 3    ## 7
10 * 3    ## 30
10 / 3    ## 3.333...
10 % 3    ## 1
10 ^ 2    ## 100  (power)
```

### Comparison

```spectator
x == y    ## equal
x != y    ## not equal
x >  y    ## greater than
x <  y    ## less than
x >= y    ## greater or equal
x <= y    ## less or equal
```

### Logical

```spectator
a && b    ## and
a || b    ## or
!a        ## not
```

### Concatenation

```spectator
"hello" --> " " --> "world"   ## "hello world"
"port " --> str(80)            ## "port 80"
```

---

## Collections

### Lists

```spectator
## Create
tools = ["nmap", "burp", "nikto"]
mixed = [1, "two", true, nil]
empty = []

## Access
tools[0]              ## "nmap"
tools[len(tools)-1]   ## "nikto"

## Modify
tools = append(tools, "gobuster")

## Functions
len(tools)                    ## 4
slice(tools, 0, 2)            ## ["nmap", "burp"]
unique([1, 2, 2, 3, 3])       ## [1, 2, 3]
reverse(tools)                ## ["gobuster","nikto","burp","nmap"]
sortList(["z","a","m"])       ## ["a","m","z"]
tally(["a","b","a","c","a"])  ## {"a":3,"b":1,"c":1}

## Set operations
diff([1,2,3,4], [2,4])        ## [1, 3]
intersect([1,2,3], [2,3,4])   ## [2, 3]
gather([[1,2],[3,4],[5]])      ## [1, 2, 3, 4, 5]
```

### Maps

```spectator
## Create
info = {
  "host":  "target.com",
  "port":  443,
  "https": true
}

## Access — both syntaxes work
info["host"]    ## bracket notation
info.host       ## dot notation

## Assign
info["proto"] = "https"
info.status   = "active"

## Iterate
each value, key : info {
  Trace(key --> " = " --> str(value))
}
```

---

## Control Flow

### If / Elseif / Else

```spectator
score = 87

if score >= 90 {
  Trace("Grade: A")
} elseif score >= 75 {
  Trace("Grade: B")
} elseif score >= 60 {
  Trace("Grade: C")
} else {
  Trace("Grade: F")
}
```

### Match

```spectator
status = "open"

match status {
  "open"     => { Trace("Port is open")     }
  "closed"   => { Trace("Port is closed")   }
  "filtered" => { Trace("Port is filtered") }
  _          => { Trace("Unknown status")   }
}
```

### Loop (Counted)

```spectator
## _i is the built-in loop counter (0-indexed)
loop 5 {
  Trace("Iteration: " --> str(_i))
}
```

### Loop (Infinite + Break)

```spectator
n = 0
loop {
  n += 1
  if n >= 10 { break }
}
```

### Each (List)

```spectator
ports = [80, 443, 8080, 8443]

each port, idx : ports {
  Trace(str(idx) --> ": " --> str(port))
}

## Index is optional
each port : ports {
  Trace(str(port))
}
```

### Each (Map)

```spectator
info = {"host": "target.com", "port": "443"}

each value, key : info {
  Trace(key --> " → " --> str(value))
}
```

---

## Functions

### Definition

```spectator
func greet(name, title) {
  return "Hello, " --> title --> " " --> name --> "!"
}

result = greet("Alice", "Dr.")
Trace(result)
```

### Recursion

```spectator
func factorial(n) {
  if n <= 1 { return 1 }
  return n * factorial(n - 1)
}

Trace(str(factorial(10)))   ## 3628800
```

### Anonymous Functions

```spectator
double = func(x) { return x * 2 }
Trace(str(double(21)))   ## 42
```

### Functions as Arguments

```spectator
func apply(value, fn) {
  return fn(value)
}

result = apply(5, func(x) { return x * x })
Trace(str(result))   ## 25
```

---

## Error Handling

### Try / Catch

```spectator
try {
  result = http("GET", "https://target.com", {"timeout": 3000})
} catch e {
  Trace("Request failed: " --> e)
}
```

### Throw

```spectator
func divide(a, b) {
  if b == 0 {
    throw("Division by zero")
  }
  return a / b
}

try {
  result = divide(10, 0)
} catch e {
  Trace("Error: " --> e)
}
```

---

## Concurrency

### Spawn (Goroutine)

```spectator
func scanHost(host) {
  ips = resolve(host)
  Trace(host --> ": " --> join(ips, ", "))
}

## Run in background — non-blocking
spawn scanHost("target1.com")
spawn scanHost("target2.com")
spawn scanHost("target3.com")

## Main continues immediately
sleep(2000)   ## wait for spawned tasks
```

---

## Shell Execution

### Capture Output

```spectator
result = # runner<?whoami>
Trace(trim(result))

output = # runner<?nmap -sV target.com>
Trace(output)
```

### Pipe Commands

```spectator
open_ports = # runner<?nmap -p- target> | # runner<?grep open>
Trace(open_ports)
```

### Redirect to File

```spectator
# runner<?nmap target.com > /tmp/scan.txt>
content = readFile("/tmp/scan.txt")
```

---

## Imports

```spectator
#Import coffee    ## Load the Coffee recon library
#Import ghost     ## Load the Ghost OSINT library
#Import Spec.GUI  ## Load the built-in GUI framework
```

---

## Built-in Helpers

### Math

```spectator
abs(-5)        ## 5
min(3, 7)      ## 3
max(3, 7)      ## 7
floor(3.7)     ## 3
ceil(3.2)      ## 4
round(3.5)     ## 4
sqrt(16)       ## 4
pow(2, 10)     ## 1024
rand()         ## random float 0.0–1.0
```

### File I/O

```spectator
writeFile("output.txt", "scan results\n")
appendFile("output.txt", "more data\n")
content = readFile("output.txt")
if exists("output.txt") { Trace("found") }
```

### JSON

```spectator
obj  = {"host": "target.com", "ports": [80, 443]}
json = jsonStr(obj)      ## serialize
data = jsonParse(json)   ## deserialize
Trace(data["host"])
```

### Time

```spectator
Trace(timestamp())    ## "20260322_154301"
sleep(1000)           ## sleep 1 second
```

### Network

```spectator
isIP("8.8.8.8")             ## true
isIP("notanip")             ## false
cidrHosts("192.168.1.0/30") ## [192.168.1.0, 192.168.1.1, ...]
```

### Display

```spectator
Trace("message")                      ## print with newline
colorize("text", "red")               ## ANSI colored text
banner("SECTION TITLE")               ## ASCII banner box
table(["Col1","Col2"], [["a","b"]])   ## ASCII table
progress(current, total, "label")     ## progress bar
```

### Input

```spectator
name   = Capture("Enter your name: ")
target = Capture("Target: ")
```
