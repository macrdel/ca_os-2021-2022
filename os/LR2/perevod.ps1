function check_str ([string]$str) {
    [int]$k_dot = 0    # кол-во точек
    [int]$k_minus = 0  # кол-во минусов
    [int]$k_plus = 0   # кол-во плюсов
    $charr = $str.ToCharArray()
    foreach ($ch in $charr) {
        if ($ch -eq '.') {
            $k_dot++
        }
        elseif (($ch -eq '-') -or ($ch -eq '+'))
        {
           if ($ch -ne $charr[0])
           {
                return $false
           }
           else {
                $k_minus++
                $k_plus++
           }
        }
        if ((($ch -lt '0') -and ($ch -ne '.') -and ($ch -ne '-') -and ($ch -ne '+')) -or ($ch -gt '9') -or ($k_dot -gt 1) -or ($k_minus -gt 1) -or ($k_plus -gt 1)) {
            return $false
        }
    }
    return $true
}

function read_data () {
    [string[]]$result = @()
    $col = Read-Host "Введите кол-во чисел"
    for ([int]$i = 1; $i -le $col; $i++) {
        [string]$tmp = Read-Host "Введите $i-ое число"
        [bool]$flag = check_str $tmp
        if ($flag -eq $true) {
            $result += $tmp
        }
    }
    return $result
}

function convert_strs_to_double ($strsarr) {
    [double[]]$result = @()
    foreach ($str in $strsarr) {
        $tmp = [double]$str
        $result += $tmp
    }
    return $result
}

function convert_int_part ($number) {
    [int]$result = 0
    if ($number -lt 3) {
        $result = $number
    }
    else {
       [int[]]$moduls = @()
       while ($number -gt 2) {
            $moduls += $number % 3
            $number = [int][Math]::Floor($number / 3)
       }
       $moduls += $number
       [int]$k = 0
       foreach ($digit in $moduls) {
            if ($digit -ne 0) {
                $result += ($digit * [Math]::Pow(10, $k))
            }
            $k++
       }
    }
    return $result
}

function convert_frac_part ($frac) {
    [double]$result = 0.0
    if ($frac -gt 0.0) {
        [int]$i = 1
        while ($i -le 8) {
            $frac *= 3
            [double]$tmp = 0.0
            while ($frac -gt 1.0) {
                $frac--
                $tmp++
            }
            if ($tmp -ne 0.0) {
                $result += $tmp * [Math]::Pow(10, (-1) * $i)
            }
            $i++
        }
    }
    return $result
}

function convert_numbers ($numbers) {
    [double[]]$result = @()
    foreach ($number in $numbers) {
        if ($number -lt 0) {
            $number = [Math]::Abs($number)
        }
        [double]$frac_part = $number
        while ($frac_part -ge 1) {
            $frac_part--
        }
        $int_part = [int]($number - $frac_part)
        $int_part = convert_int_part $int_part
        $frac_part = convert_frac_part $frac_part
        $result += [double]($int_part + $frac_part)
    }
    return $result
}

function output($correct_numbers, $converted_numbers) {
    Write-Host "Output "
    for ([int]$i = 0; $i -lt $correct_numbers.Count; $i++) {
        [double]$d = $converted_numbers[$i]
        if ($correct_numbers[$i] -lt 0) {
            $d = -$d
        }
        Write-Host $correct_numbers[$i] "->" $d
    }
}


[string[]]$input_strs = read_data
if ($input_strs.count -eq 0) {
    Write-Host "All data is uncorrect!"
}
else {
    $correct_numbers = convert_strs_to_double $input_strs
    $converted_numbers = convert_numbers $correct_numbers
    output $correct_numbers $converted_numbers
}