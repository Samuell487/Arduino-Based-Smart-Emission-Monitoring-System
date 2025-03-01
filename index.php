<?php
$hostname = "localhost"; 
$username = "root"; 
$password = ""; 
$database = "trial_1"; 

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) { 
    die("Connection failed: " . mysqli_connect_error()); 
} 

echo "Database connection is OK<br>"; 

if(isset($_POST["temperature"]) && isset($_POST["humidity"]) && isset($_POST["result"])) {
    $t = $_POST["temperature"];
    $h = $_POST["humidity"];
    $g = $_POST["result"];

    $sql = "INSERT INTO experiment3 (HC, CO, RESULTS) VALUES ('$t', '$h','$g')"; 

    if (mysqli_query($conn, $sql)) { 
        echo "\nNew record created successfully"; 
    } else { 
        echo "Error: " . $sql . "<br>" . mysqli_error($conn); 
    }
}

?>

<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Dynamic Table with User Input</title>
<style>
  table {
    width: 80%;
    border-collapse: collapse;
    margin: 20px auto;
  }
  th, td {
    border: 1px solid black;
    padding: 12px;
    text-align: center;
  }
  th {
    background-color: #f2f2f2;
  }
</style>
</head>
<body>

<h2>Enter Number Plate:</h2>
<input type="text" id="numberPlateInput">
<button onclick="if(document.getElementById('numberPlateInput').value.trim() !== '') { fetchDataAndCreateTableRow(); clearInput(); } else { alert('Please enter a number plate.'); }">Create Table Row</button>

<table id="dataTable">
  <thead>
    <tr>
      <th>Number Plate</th>
      <th>CO (%)</th>
      <th>HC (ppm)</th>
      <th>Results</th>
    </tr>
  </thead>
  <tbody>
    <!-- Table body will be populated dynamically -->
  </tbody>
</table>

<script>
// Initialize a counter to keep track of the last fetched row
var rowCounter = 0;

function fetchDataAndCreateTableRow() {
    var numberPlate = document.getElementById('numberPlateInput').value;
    var tableBody = document.querySelector('#dataTable tbody');
    
    // Fetch data from the PHP script using AJAX
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                var data = JSON.parse(xhr.responseText);
                
                // If there are no more rows to fetch, return
                if (rowCounter >= data.length) {
                    console.log('No more rows to fetch');
                    return;
                }
                
                // Get the next row data
                var rowData = data[rowCounter++];
                
                // Create table row with fetched data
                var row = document.createElement('tr');
                
                // Append user input for Number Plate column
                var numberPlateCell = document.createElement('td');
                numberPlateCell.textContent = numberPlate;
                row.appendChild(numberPlateCell);
                
                // Append fetched CO value to CO column
                var coCell = document.createElement('td');
                coCell.textContent = rowData['CO']; // Display CO value from database
                row.appendChild(coCell);
                
                // Append fetched HC value to HC column
                var hcCell = document.createElement('td');
                hcCell.textContent = rowData['HC']; // Display HC value from database
                row.appendChild(hcCell);
                
                // Append fetched datetime value to Results column
                var resultsCell = document.createElement('td');
                resultsCell.textContent = rowData['RESULTS']; // Display datetime value from database
                row.appendChild(resultsCell);
                
                tableBody.appendChild(row);
            } else {
                console.error('Error fetching data:', xhr.status);
            }
        }
    };
    xhr.open('GET', 'fetch_data.php', true);
    xhr.send();
}

function clearInput() {
    document.getElementById('numberPlateInput').value = "";
}
</script>


</body>
</html>
