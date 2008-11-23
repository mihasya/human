<?php
echo "Testing human_interval_precies; left hand side should equal right hand side\n";
$seconds = array(
    '1m' => 60,
    '2m' => 120,
    '2m 5s' => 125,
    '59m' => 3540,
    '1h' => 3600,
    '1h 1m 1s' => 3661,
    '1d 1m 2s' => 3600*24 + 62,
);
foreach ($seconds as $k=>$v) {
    $human = human_interval_precise($v);
    echo "$k : $human\n";
}
?>
