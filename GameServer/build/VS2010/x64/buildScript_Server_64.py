import subprocess

output_binary = subprocess.check_output(['cmake', '-G', 'Visual Studio 10 Win64', '..\..\..\src'])
output = output_binary.decode(encoding='utf-8')
print(output)
key = input('Press any key to continue...')
