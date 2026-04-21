# ============================================================
# app.py — Flask Web Server (Updated with Parse Tree support)
# ============================================================

from flask import Flask, render_template, request, jsonify
import subprocess
import os

app = Flask(__name__)

BASE_DIR      = os.path.dirname(os.path.abspath(__file__))
COMPILER_PATH = os.path.join(BASE_DIR, "compiler", "compiler.exe")


# ============================================================
# parse_output(raw)
# Splits compiler stdout into 5 sections using ===MARKERS===
# ============================================================
def parse_output(raw):

    result = {
        "tokens"   : "(no output)",
        "parse"    : "(no outp33ut)",
        "symtable" : "(no output)",
        "tac"      : "(no output)",
        "tree"     : "{}",
    }

    sections = {
        "tokens"   : "===TOKENS===",
        "parse"    : "===PARSE===",
        "symtable" : "===SYMTABLE===",
        "tac"      : "===TAC===",
        "tree"     : "===TREE===",
    }

    for key, marker in sections.items():
        if marker in raw:
            after = raw.split(marker, 1)[1]
            next_markers = [m for m in sections.values() if m != marker]
            end = len(after)
            for nm in next_markers:
                if nm in after:
                    pos = after.index(nm)
                    if pos < end:
                        end = pos
            result[key] = after[:end].strip()

    return result


# ============================================================
# index() — GET /
# ============================================================
@app.route("/")
def index():
    return render_template("index.html")


# ============================================================
# compile() — POST /compile
# ============================================================
@app.route("/compile", methods=["POST"])
def compile():

    data = request.get_json()
    if not data or "code" not in data:
        return jsonify({
            "success" : False,
            "error"   : "No code received."
        }), 400

    code = data["code"].strip()

    if not code:
        return jsonify({
            "success" : False,
            "error"   : "Please enter some C code."
        }), 400

    if not os.path.exists(COMPILER_PATH):
        return jsonify({
            "success" : False,
            "error"   : f"compiler.exe not found at: {COMPILER_PATH}\n"
                        f"Please run mingw32-make inside the compiler/ folder first."
        }), 500

    try:
        result = subprocess.run(
            [COMPILER_PATH],
            input          = code,
            capture_output = True,
            text           = True,
            timeout        = 10,
            cwd            = os.path.join(BASE_DIR, "compiler")
        )

        raw_output = result.stdout
        if result.stderr:
            raw_output += "\n" + result.stderr

        sections = parse_output(raw_output)

        return jsonify({
            "success"  : True,
            "tokens"   : sections["tokens"],
            "parse"    : sections["parse"],
            "symtable" : sections["symtable"],
            "tac"      : sections["tac"],
            "tree"     : sections["tree"],
            "raw"      : raw_output
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "success" : False,
            "error"   : "Compiler timed out (infinite loop in your code?)"
        }), 500

    except Exception as e:
        return jsonify({
            "success" : False,
            "error"   : f"Server error: {str(e)}"
        }), 500


# ============================================================
# Run Flask
# ============================================================
if __name__ == "__main__":
    print("=" * 50)
    print("  Mini C Compiler Web App")
    print("=" * 50)
    print(f"  Compiler path : {COMPILER_PATH}")
    print(f"  Compiler found: {os.path.exists(COMPILER_PATH)}")
    print(f"  Open browser  : http://localhost:5000")
    print("=" * 50)

    app.run(
        debug = True,
        port  = 5000,
        host  = "0.0.0.0"
    )