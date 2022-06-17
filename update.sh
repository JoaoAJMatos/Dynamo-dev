echo "[+] Updating from latest remote changes..."

SOURCE="${BASH_SOURCE[0]:-$0}";

while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
  SOURCE="$(readlink "$SOURCE")";
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"; # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done

DIR="$(cd -P "$(dirname -- "$SOURCE"; )" &> /dev/null && pwd 2> /dev/null; )";

echo "[+] Found project root directory: $DIR"

# Navigate to the root of the project
cd "$DIR"
cd ../

echo "[+] Deleting old files..."
rm -r -f "Dynamo-dev"
echo "[+] Cloning latest version..."
git clone "https://github.com/JoaoAJMatos/Dynamo-dev.git"
cd "Dynamo-dev"
mkdir "build"
cd "build"
echo "[+] Building..."
cmake ..
make