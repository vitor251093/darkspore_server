cd docker
docker build -t recap_builder .
docker run recap_builder > ../recap_builder
chmod +x ../recap_builder 
cd ..

./recap_builder bash -c './prepare.sh'
