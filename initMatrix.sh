echo "Generating files..."
cd src
rm -rf input
mkdir input
python3 matrixGenerator.py
echo "finished"