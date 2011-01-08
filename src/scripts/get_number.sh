#!/bin/bash
$HOME/bin/ongoingnc | grep -P "${1}\s+(?=\d+/[\d?])" | grep -Po '\d+(?=/[\d?])'
