# stego-toolkit

Docker image for steganalysis/CTFs

## Included tools

* outguess
* stegosuite
* steghide
* jphs (jphide/jpseek/jpcrack)
	* jpcrack accepts a stegged file and a wordlist to attempt to crack the password with
* snow (stegsnow)
* zsteg
* stegdetect

## Shell scripts

* `build.sh` - build docker image
* `run.sh` - run docker container and mount $1 (directory to analyze) to `/puzzles`

