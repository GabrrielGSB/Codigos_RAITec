import subprocess
result = subprocess.run(["echo", "Subprocess está funcionando!"], capture_output=True, text=True)
print(result.stdout)
