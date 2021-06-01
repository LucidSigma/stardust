$archives = Get-ChildItem -Path .\ -Include *.pak -File -Name

$archives | ForEach-Object {
	Write-Output "Deleting old $_..."
	Remove-Item $_
}

"assets", "locales" | ForEach-Object {
	Write-Output "Compressing $_.pak..."
	Compress-Archive -Path ".\$_" -DestinationPath ".\$_.zip" -CompressionLevel Optimal
	Rename-Item -Path ".\$_.zip" -NewName ".\$_.pak"
}

Write-Output "New archives created successfully."
Read-Host