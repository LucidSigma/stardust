"assets", "locales" | ForEach-Object {
	if (Test-Path ".\$_.pak" -PathType Leaf) {
		Write-Output "Deleting old $_..."
		Remove-Item ".\$_.pak"
	}

	Write-Output "Compressing $_.pak..."
	Compress-Archive -Path ".\$_" -DestinationPath ".\$_.zip" -CompressionLevel Optimal
	Rename-Item -Path ".\$_.zip" -NewName ".\$_.pak"
}

Write-Output "New archives created."
Read-Host