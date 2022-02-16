chmod 0644 infile.txt
rm -f outfile.txt
< infile.txt ls -l | wc -l > outfile.txt
cat outfile.txt
rm outfile.txt
./pipex infile.txt "ls -l" "wc -l" outfile.txt
cat outfile.txt
rm -f outfile.txt

./pipex infile.txt "grep a1" "wc -w" outfile.txt
cat outfile.txt
rm outfile.txt
< infile.txt grep a1 | wc -w > outfile.txt
cat outfile.txt
rm outfile.txt

./pipex
./pipex infile.txt
./pipex yo lo
./pipex yo lo yo
./pipex wrong_name "grep a1" "cat" outfile.txt
./pipex infile.txt yo cat outfile.txt
./pipex infile.txt cat yo outfile.txt
./pipex infile.txt echo cat wrong_file

chmod 0644 infile.txt
chmod -r infile.txt
< infile.txt echo | cat > outfile.txt
./pipex infile.txt echo cat outfile.txt
chmod -w infile.txt
< infile.txt echo | cat > outfile.txt
./pipex infile.txt echo cat outfile.txt

touch outfile.txt 
chmod 0644 outfile.txt
chmod -r outfile.txt
< infile.txt echo | cat > outfile.txt
./pipex infile.txt echo cat outfile.txt
chmod -w outfile.txt
< infile.txt echo | cat > outfile.txt
./pipex infile.txt echo cat outfile.txt
