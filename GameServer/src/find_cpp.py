import fnmatch
import os

for root, dirnames, filenames in os.walk('bullet'):
	for filename in fnmatch.filter(filenames, '*.cpp'):
		print(os.path.join(root, filename))

