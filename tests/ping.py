import subprocess


subprocess.run(["ping", "-c", "4", "google.com"])
result = subprocess.run(["ping", "-c", "4", "google.com"], capture_output=True, text=True)
print("Лог ответа сервера:\n", result.stdout)