import subprocess
import time
import json
from pathlib import Path

MODULE = "test_sysfs.ko"
SYSFS_PATH = "/sys/kernel/jovin_test/value"

def run(cmd):
	result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
	return result.returncode, result.stdout.strip(),result.stderr.strip()

def build_module():
	return run("make")

def load_module():
	return run(f"sudo insmod {MODULE}")

def unload_module():
	return run(f"sudo rmmod {MODULE}")

def write_value(v):
	return run(f"echo {v} | sudo tee {SYSFS_PATH}")

def read_value():
	code , out, err = run(f"cat {SYSFS_PATH}")
	return code,out.strip(),err

def main():
	report = {"steps": []}

	for name, func in [
	  ("build",build_module),
	  ("load",load_module),
	  ("write", lambda: write_value(42)),
	  ("read", read_value),
	  ("unload", unload_module),
	]:
		code, out, err = func()
		report["steps"].append({
		  "step": name,
		  "return_code": code,
		  "stdout": out,
		  "stderr": err
		})
		if code != 0:
		  break

	Path("report.json").write_text(json.dumps(report, indent=2))
	print("Test Complete.Report saved to report.json")

if __name__ == "__main__":
  main()

	
